#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_logger.h"
#include "mock_node_manager.h"
#include "bs_tree.h"
#include <memory>
#include <vector>

using namespace ffvms;
using namespace ffvms::test;
using ::testing::Return;
using ::testing::_;
using ::testing::NiceMock;

class BSTreeTest : public ::testing::Test {
protected:
    NiceMock<MockLogger> mock_logger;
    NiceMock<MockNodeManager> mock_node_manager;
    std::unique_ptr<BSTree> tree;
    
    // Track nodes to cleanup manually since we used new
    std::vector<treeNode*> nodes;

    void SetUp() override {
        tree = std::make_unique<BSTree>(&mock_logger, &mock_node_manager);
    }

    void TearDown() override {
        for (auto* node : nodes) {
            delete node;
        }
        nodes.clear();
    }
    
    treeNode* create_node(unsigned long long link, treeNode::TYPE type = treeNode::DIR) {
        treeNode* node = new treeNode(type);
        node->link = link;
        node->cnt = 1; 
        node->first_son = nullptr;
        node->next_brother = nullptr;
        nodes.push_back(node);
        return node;
    }
    
    // Setup a correct tree structure with HEAD_NODE
    // root -> head -> child1 -> child2
    // Path initialized to [root, head]
    treeNode* setup_simple_tree() {
        treeNode* root = create_node(1, treeNode::DIR); 
        treeNode* head = create_node(99, treeNode::HEAD_NODE);
        treeNode* c1 = create_node(2, treeNode::DIR); 
        treeNode* c2 = create_node(3, treeNode::DIR); 
        
        root->first_son = head;
        head->next_brother = c1;
        c1->next_brother = c2;
        
        tree->path.push_back(root);
        tree->path.push_back(head);
        return root;
    }
};

TEST_F(BSTreeTest, GoToFindsChildNode) {
    setup_simple_tree();
    
    // HEAD_NODE check (link 99)
    EXPECT_CALL(mock_node_manager, get_name(99)).WillRepeatedly(Return(""));
    EXPECT_CALL(mock_node_manager, get_name(2))
        .WillRepeatedly(testing::Invoke([](unsigned long long id) {
            std::cout << "Mock: get_name(" << id << ") -> child1\n";
            return "child1";
        }));
    EXPECT_CALL(mock_node_manager, get_name(3)).WillRepeatedly(Return("child2"));
    
    // Go to child1
    EXPECT_TRUE(tree->go_to("child1"));
    
    // Debug Path
    std::cout << "Path size: " << tree->path.size() << "\n";
    for(size_t i=0; i<tree->path.size(); i++) {
        std::cout << "Path[" << i << "] link: " << tree->path[i]->link << "\n";
    }

    // Expectation adjustment if logic differs from theory, but keeping 3 for now to see debug output
    // If it fails again, check output.
    EXPECT_EQ(tree->path.size(), 3);
    if (tree->path.size() >= 3) {
        EXPECT_EQ(tree->path.back()->link, 2);
    }
    
    // Reset path
    tree->path.pop_back(); 
    
    // Go to child2
    EXPECT_TRUE(tree->go_to("child2"));
}

TEST_F(BSTreeTest, GoToFailsForNonexistentNode) {
    setup_simple_tree();
    
    EXPECT_CALL(mock_node_manager, get_name(99)).WillRepeatedly(Return(""));
    EXPECT_CALL(mock_node_manager, get_name(2)).WillRepeatedly(Return("child1"));
    EXPECT_CALL(mock_node_manager, get_name(3)).WillRepeatedly(Return("child2"));
    
    EXPECT_FALSE(tree->go_to("nonexistent"));
    // Side effect of name_exist() iterates to end of list: [root, head, c1, c2]
    EXPECT_EQ(tree->path.size(), 4); 
}

TEST_F(BSTreeTest, GotoTailMovesToLastBrother) {
    setup_simple_tree(); 
    // Initial path [root, head]
    
    // goto_tail moves from head to c1 to c2
    EXPECT_TRUE(tree->goto_tail());
    // Path accumulates siblings? 
    // Logic: while(next) push_back(next).
    // So [root, head, c1, c2]
    
    EXPECT_EQ(tree->path.back()->link, 3); // c2
    EXPECT_EQ(tree->path.size(), 4);
}

TEST_F(BSTreeTest, NameExistChecksCurrentDirChildren) {
    setup_simple_tree();
    
    // list_directory_contents calls goto_head, then iterates info.
    EXPECT_CALL(mock_node_manager, get_name(2)).WillRepeatedly(Return("child1"));
    EXPECT_CALL(mock_node_manager, get_name(3)).WillRepeatedly(Return("child2"));

    EXPECT_TRUE(tree->name_exist("child1"));
    EXPECT_TRUE(tree->name_exist("child2"));
    EXPECT_FALSE(tree->name_exist("child3"));
}

TEST_F(BSTreeTest, GotoHeadMovesToHead) {
    setup_simple_tree();
    // Simulate being at c2: [root, head, c1, c2]
    // Note: manual push might skip c1, but goto_tail includes all.
    // Let's manually reconstruct path that looks like we traversed
    treeNode* head = tree->path.back(); // link 99
    treeNode* c1 = head->next_brother;
    treeNode* c2 = c1->next_brother;
    
    tree->path.push_back(c1);
    tree->path.push_back(c2);
    
    EXPECT_TRUE(tree->goto_head());
    // Should pop back until head (is_son returns true for head)
    // path: [root, head]
    EXPECT_EQ(tree->path.back()->link, 99);
}

TEST_F(BSTreeTest, CheckPathFailsIfEmpty) {
    tree->path.clear();
    EXPECT_FALSE(tree->check_path());
    // Also check logging?
    EXPECT_CALL(mock_logger, log(_, _, _)).Times(1);
    tree->check_path();
}
