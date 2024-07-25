// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
using namespace std;

TEST(bst_public_test) {
  BinarySearchTree<int> tree;

  tree.insert(5);

  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);

  ASSERT_TRUE(tree.find(5) != tree.end());

  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
  ASSERT_TRUE(*tree.max_element() == 7);
  ASSERT_TRUE(*tree.min_element() == 3);
  ASSERT_TRUE(*tree.min_greater_than(5) == 7);

  cout << "cout << tree.to_string()" << endl;
  cout << tree.to_string() << endl << endl;

  cout << "cout << tree" << endl << "(uses iterators)" << endl;
  cout << tree << endl << endl;

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  cout << "preorder" << endl;
  cout << oss_preorder.str() << endl << endl;
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  cout << "inorder" << endl;
  cout << oss_inorder.str() << endl << endl;
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

TEST(test_empty){
  BinarySearchTree<int> tree;

  tree.insert(4); 
  tree.insert(10); 

  ASSERT_FALSE(tree.empty());
}
 
TEST(test_empty2){
  BinarySearchTree<int> tree;

  ASSERT_TRUE(tree.empty());
}


TEST(test_size) {
  BinarySearchTree<int> tree;

  tree.insert(5); 
  tree.insert(3);
  tree.insert(7);

  ASSERT_TRUE(tree.size()== 3);
}

TEST(test_height){ 
  BinarySearchTree<int> tree;

  tree.insert(5); 
  tree.insert(4);

  ASSERT_TRUE(tree.height() == 2);
}

TEST(test_height2){ 
  BinarySearchTree<int> tree;

  tree.insert(5); 
  tree.insert(4);
  tree.insert(9);
  tree.insert(7);

  ASSERT_TRUE(tree.height() == 3);
}

TEST(test_find){ 
  BinarySearchTree<int> tree;

  tree.insert(5); 
  tree.insert(4);
  tree.insert(7);
  tree.insert(9);
  tree.insert(0);

  ASSERT_TRUE(tree.find(4) != tree.end());
} 

TEST(test_find2){ 
  BinarySearchTree<int> tree; 

  tree.insert(5); 
  tree.insert(4);
  tree.insert(7);
  tree.insert(9);
  tree.insert(0);

  ASSERT_TRUE(tree.find(4) != tree.begin());
} 

TEST(test_min_element){
  BinarySearchTree<int> tree;

  tree.insert(5); 
  tree.insert(4);
  tree.insert(0);

  ASSERT_TRUE(*tree.min_element() == 0); 
}

TEST(test_max_element){
  BinarySearchTree<int> tree;

  tree.insert(5); 
  tree.insert(9);
  tree.insert(0);

  ASSERT_TRUE(*tree.max_element() == 9); 
}

TEST(test_min_greater_than){
  BinarySearchTree<int> tree;

  tree.insert(5); 
  tree.insert(3); 
  tree.insert(7);

  ASSERT_TRUE(*tree.min_greater_than(5) == 7); 
}

TEST(test_min_greater_than_2){
  BinarySearchTree<int> tree;

  tree.insert(12); 
  tree.insert(3); 
  tree.insert(7);
  tree.insert(33);
  tree.insert(5);
  tree.insert(11);
  tree.insert(6);

  ASSERT_TRUE(*tree.min_greater_than(6) == 7); 
}

TEST(test_min_greater_than_3){
  BinarySearchTree<int> tree;
  BinarySearchTree<int> tree2;

  tree.insert(12); 
  tree.insert(3); 
  tree.insert(7);
  tree2 = tree;

  ASSERT_EQUAL(tree.size(), 3);
  ASSERT_EQUAL(tree2.size(), 3);

  ASSERT_EQUAL(*tree.min_greater_than(2), 3);
  ASSERT_EQUAL(tree.size(), tree2.size());

  ASSERT_TRUE(!tree.empty());
  
  tree.insert(5);
  tree.insert(11);
  tree.insert(6);

  ASSERT_EQUAL(*tree.min_greater_than(5), 6);
  ASSERT_TRUE(tree.size() != tree2.size());
}

TEST(order1){
  BinarySearchTree<int> tree;
  ostringstream order1;

  ASSERT_TRUE(order1.str() == "");

  tree.insert(1);
  tree.insert(2);
  tree.insert(3);

  tree.traverse_inorder(order1);

  ASSERT_FALSE(order1.str() == "1 2 3")

}

TEST(order2){
  BinarySearchTree<int> tree;
  ostringstream order2;
  
  ASSERT_TRUE(order2.str() == "");

  tree.insert(1);
  tree.insert(2);
  tree.insert(3);

  tree.traverse_preorder(order2);

  ASSERT_FALSE(order2.str() == "1 2 3")
}

TEST(test_checking_sorting_invariant){
  BinarySearchTree<int> tree;

  tree.insert(12); 
  tree.insert(3); 
  tree.insert(13);

  ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST_MAIN()