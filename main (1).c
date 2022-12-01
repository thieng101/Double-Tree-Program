/* COP 3502C Programming Assignment 3
This program is written by: Hong Thy Nguyen */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

FILE *fptr;

#define MAXLEN 50

typedef struct itemNode {
  char name[MAXLEN];
  int count;
  struct itemNode *left, *right;
} itemNode;

typedef struct treeNameNode {
  char treeName[MAXLEN];
  struct treeNameNode *left, *right;
  itemNode *theTree;
} treeNameNode;

treeNameNode *createTreeNameNode(char name[]) {
  treeNameNode *node = malloc(sizeof(treeNameNode));
  strcpy(node->treeName, name);
  node->left = NULL;
  node->right = NULL;
  node->theTree = NULL;

  return node;
}

treeNameNode *insert_tree_name_node(treeNameNode *tree_root,
                                    treeNameNode *tree_node) {
  if (tree_root == NULL) {
    tree_root = tree_node;
  }
  int res = strcmp(tree_node->treeName, tree_root->treeName);
  if (res < 0)
    tree_root->left = insert_tree_name_node(tree_root->left, tree_node);
  if (res > 0)
    tree_root->right = insert_tree_name_node(tree_root->right, tree_node);

  return tree_root;
}

treeNameNode *buildNameTree(treeNameNode *tree_names_root,
                            int num_of_tree_names) {
  // treeNameNode* node;
  while (num_of_tree_names != 0) {
    char tree_name[MAXLEN];
    scanf("\n%s", tree_name);

    treeNameNode *node = NULL;
    node = createTreeNameNode(tree_name);
    tree_names_root = insert_tree_name_node(tree_names_root, node);

    num_of_tree_names--;
  }

  return tree_names_root;
}

treeNameNode *searchTreeNameRoot(treeNameNode *tree_name_root,
                                 char input_name[MAXLEN]) {

  if (tree_name_root == NULL)
    return tree_name_root;

  if (strcmp(input_name, tree_name_root->treeName) < 0) {
    return searchTreeNameRoot(tree_name_root->left, input_name);
  } else if (strcmp(input_name, tree_name_root->treeName) > 0) {
    return searchTreeNameRoot(tree_name_root->right, input_name);
  } else {
    return tree_name_root;
  }
}

itemNode *createItemNode(char name[], int count) {
  itemNode *node = malloc(sizeof(itemNode));
  node->count = count;
  strcpy(node->name, name);
  node->left = NULL;
  node->right = NULL;

  return node;
}

itemNode *insertItemNode(itemNode *item_root, itemNode *item_node) {
  if (item_root == NULL) {
    return item_node;
  }
  int res = strcmp(item_node->name, item_root->name);
  if (res < 0)
    item_root->left = insertItemNode(item_root->left, item_node);
  else
    item_root->right = insertItemNode(item_root->right, item_node);

  return item_root;
}

void builItemTree(treeNameNode *tree_name_root, int total_items) {

  char tree_name[MAXLEN];
  char item_name[MAXLEN];
  int count;
  treeNameNode *correct_root;
  itemNode *node;

  while (total_items != 0) {
    scanf("%s %s %d", tree_name, item_name, &count);
    // printf("%s %s %d\n", treeName, itemName, count);
    correct_root = searchTreeNameRoot(tree_name_root, tree_name);

    node = createItemNode(item_name, count);

    correct_root->theTree = insertItemNode(correct_root->theTree, node);

    total_items--;
  }
}

void traverse_item_tree(itemNode *root) {
  if (root != NULL) {
    traverse_item_tree(root->left);
    printf("%s ", root->name);
    fprintf(fptr,"%s ", root->name);
    traverse_item_tree(root->right);
  }
}


void print_tree_name(treeNameNode *root) {

  if (root != NULL) {
    print_tree_name(root->left);
    printf("%s ", root->treeName);
    fprintf(fptr, "%s ", root->treeName);
    print_tree_name(root->right);
  }
}

void traverse_in_traverse(treeNameNode *root) {

  if (root != NULL) {
    traverse_in_traverse(root->left);
    printf("\n==%s==\n", root->treeName);
    fprintf(fptr,"\n==%s==\n", root->treeName);
    traverse_item_tree(root->theTree);
    traverse_in_traverse(root->right);
  }
}

int isLeaf(itemNode* node) {
    return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChild(itemNode* node) {
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChild(itemNode* node) {
    return (node->left == NULL && node->right != NULL);
}

itemNode* parent(itemNode* root, itemNode* node) {

    // Take care of NULL cases.
    if (root == NULL || root == node)
        return NULL;

    // The root is the direct parent of node.
    if (root->left == node || root->right == node)
        return root;

    // Look for node's parent in the left side of the tree.
    if (strcmp(node->name,root->name) < 0)
        return parent(root->left, node);

    // Look for node's parent in the right side of the tree.
    else if (strcmp(node->name,root->name) > 0)
        return parent(root->right, node);

    return NULL; // Catch any other extraneous cases.
}

itemNode* minVal(itemNode*root) {

    // Root stores the minimal value.
    if (root->left == NULL)
        return root;

    // The left subtree of the root stores the minimal value.
    else
        return minVal(root->left);
}

itemNode *search_item_in_tree_name(itemNode *the_tree_ptr, char input[MAXLEN]) {
  if (the_tree_ptr == NULL)
    return NULL;

  if (strcmp(input, the_tree_ptr->name) < 0) {
    return search_item_in_tree_name(the_tree_ptr->left, input);
  } else if (strcmp(input, the_tree_ptr->name) > 0) {
    return search_item_in_tree_name(the_tree_ptr->right, input);
  } else {
    return the_tree_ptr;
  }
}

itemNode* deletion(itemNode* root,char input[MAXLEN]) {
    itemNode* delNode, * newDelNode, * saveNode;
    itemNode* par;
    char saveVal[MAXLEN];

    delNode = search_item_in_tree_name(root,input);
  
    par = parent(root, delNode); //Get the parent of this node

    //Take care of the case where the node to delete is a leaf node
    if (isLeaf(delNode)) {

        //Delete the only node in the tree
        if (par == NULL) {
            free(root);
            return NULL;
        }

        //Delete the node if it's a left child
        if (strcmp(delNode->name,par->name) < 0) {
            free(par->left);
            par->left = NULL;
        }
        else {
            free(par->right);
            par->right = NULL;
        }
        strcpy(delNode->name, "");
        return root; //return the root of the new tree

    }

    //Take care of the case where the node to be deleted only has a left child
    if (hasOnlyLeftChild(delNode)) {
        if (par == NULL) {
            saveNode = delNode->left;
            free(delNode);
            return saveNode;
        }

        //Deletes the node if it's a left child.
        if (strcmp(delNode->name, par->name) < 0) {
            saveNode = par->left;
            par->left = par->left->left;
            free(saveNode);
        }
        else {
            saveNode = par->right;
            par->right = par->right->left;//??
            free(saveNode);

        }
        return root;
    }

    if (hasOnlyRightChild(delNode)) {

        // Node to delete is the root node.
        if (par == NULL) {
            saveNode = delNode->right;
            free(delNode);
            return saveNode;
        }

        // Delete's the node if it is a left child.
        if (strcmp(delNode->name, par->name) < 0) {
            saveNode = par->left;
            par->left = par->left->right;
            free(saveNode);
        }

        // Delete's the node if it is a right child.
        else {
            saveNode = par->right;
            par->right = par->right->right;
            free(saveNode);
        }

        // This is the resulting root.
        return root;
    }

    //this part is wrong
    //If your code reaches here, it means your node has two children
    // Find the new physical node to delete.
    newDelNode = minVal(delNode->right);

    //save name and count data
    int saveData = newDelNode->count;
    strcpy(saveVal, newDelNode->name);
  
    deletion(root, saveVal);  // Now, delete the proper value.

    // Restore the data to the original node to be deleted.
    strcpy(delNode->name, saveVal);
    delNode->count = saveData;

    return root;
  

}

int item_before(itemNode* root, char input[MAXLEN]) {
    if (root == NULL)
        return 0;
    else {
        if (strcmp(root->name, input) < 0) {
            return 1 + item_before(root->left, input) + item_before(root->right, input);
        }
        else {
            return item_before(root->left,input);
        }
    }
}

int count(itemNode* root) {
  if (root == NULL)
      return 0;
  else
      return root->count + count(root->left) + count(root->right);
}

itemNode* reduce(itemNode* root,char input[MAXLEN], int reduce_amount) {
  
  itemNode* node = search_item_in_tree_name(root,input);
  
  if ((node->count - reduce_amount) <= 0) {
      root = deletion(root, input);
      return root;
  }

  node->count = node->count - reduce_amount;

  return root;
}

void deleteAllItems(itemNode* node) {
    if (node == NULL) return;

    //first delete both subtrees 
    deleteAllItems(node->left);
    deleteAllItems(node->right);

    // then delete the node 
    free(node);
}

void freeTreeNames(treeNameNode* node){
      if (node == NULL) return;

    //first delete both subtrees 
    freeTreeNames(node->left);
    freeTreeNames(node->right);
    deleteAllItems(node->theTree);

    // then delete the node 
    free(node);
}

int isLeaf_TreeName(treeNameNode* node) {
    return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChild_TreeName(treeNameNode* node) {
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChild_TreeName(treeNameNode* node) {
    return (node->left == NULL && node->right != NULL);
}

treeNameNode* parent2(treeNameNode* root, treeNameNode* node) {

    // Take care of NULL cases.
    if (root == NULL || root == node)
        return NULL;

    // The root is the direct parent of node.
    if (root->left == node || root->right == node)
        return root;

    // Look for node's parent in the left side of the tree.
    if (strcmp(node->treeName, root->treeName) < 0)
        return parent2(root->left, node);

    // Look for node's parent in the right side of the tree.
    else if (strcmp(node->treeName, root->treeName) > 0)
        return parent2(root->right, node);

    return NULL; // Catch any other extraneous cases.
}

treeNameNode* minVal2(treeNameNode* root) {

    // Root stores the minimal value.
    if (root->left == NULL)
        return root;

    // The left subtree of the root stores the minimal value.
    else
        return minVal2(root->left);
}

treeNameNode* deletion2(treeNameNode* root, char input[MAXLEN]) {
    if (root == NULL)
        return root;

    treeNameNode* delNode, * newDelNode, * saveNode;
    treeNameNode* par;
    char saveVal[MAXLEN];

    delNode = searchTreeNameRoot(root,input);

    par = parent2(root, delNode); //Get the parent of this node?

    //Take care of the case where the node to delete is a leaf node
    if (isLeaf_TreeName(delNode)) {

        //Delete the only node in the tree
        if (par == NULL) {
            free(root);
            return NULL;
        }

        //Delete the node if it's a left child
        if (strcmp(delNode->treeName, par->treeName) < 0) {
            free(par->left);
            par->left = NULL;
        }
        else {
            free(par->right);
            par->right = NULL;
        }
        return root; //return the root of the new tree

    }

    //Take care of the case where the node to be deleted only has a left child
    if (hasOnlyLeftChild_TreeName(delNode)) {
        if (par == NULL) {
            saveNode = delNode->left;
            free(delNode);
            return saveNode;
        }

        //Deletes the node if it's a left child.
        if (strcmp(delNode->treeName, par->treeName) < 0) {
            saveNode = par->left;
            par->left = par->left->left;
            free(saveNode);
        }
        else {
            saveNode = par->right;
            par->right = par->right->left;//??
            free(saveNode);

        }
        return root;
    }

    if (hasOnlyRightChild_TreeName(delNode)) {

        // Node to delete is the root node.
        if (par == NULL) {
            saveNode = delNode->right;
            free(delNode);
            return saveNode;
        }

        // Delete's the node if it is a left child.
        if (strcmp(delNode->treeName, par->treeName) < 0) {
            saveNode = par->left;
            par->left = par->left->right;
            free(saveNode);
        }

        // Delete's the node if it is a right child.
        else {
            saveNode = par->right;
            par->right = par->right->right;
            free(saveNode);
        }

        // This is the resulting root.
        return root;
    }
    // Find the new physical node to delete.
    newDelNode = minVal2(delNode->right);

    strcpy(saveVal, newDelNode->treeName);
  
    deletion2(root, saveVal);  // Now, delete the proper value.

    // Restore the data to the original node to be deleted.
    strcpy(delNode->treeName, saveVal);

    return root;

}

int calLeft(itemNode *root){
  int left = 0;

  //go to the last node of the left side
  while (root->left != NULL){
    left ++;
    root = root->left;
  }

  //check the right side to add up if neccessary
  if(root->right != NULL){
    while(root->right != NULL){
      left++;
      root = root->right;
    }
  }
  return left;
}

int calRight(itemNode *root){
  int right = 0;

  //go to the last node of the right side node
  while (root->right != NULL){
    right ++;
    root = root->right;
  }

  //check the left side (of right side node) to add up if neccessary
  if(root->left != NULL){
    while(root->left != NULL){
      right++;
      root = root->left;
    }
  }
  return right;
}

void heightBalance(treeNameNode* tree_name_node,itemNode* root){
  int left,right,diff;

  if(root->left == NULL){
    left = -1;
  }

  if(root->right == NULL){
    right = -1;
  }

  if(root->left != NULL){
    left = calLeft(root->left);
  }

  if(root->right != NULL){
    right = calRight(root->right);
  }
  
  //calcualte absolute value
  diff = abs(right - left);

  if(diff == 0){
  printf("\n%s: left height %d, right height %d, difference %d, balanced", tree_name_node->treeName, left, right, diff);
  fprintf(fptr,"\n%s: left height %d, right height %d, difference %d, balanced", tree_name_node->treeName, left, right, diff);  
  }else{
  printf("\n%s: left height %d, right height %d, difference %d, not balanced", tree_name_node->treeName, left, right, diff);
  fprintf(fptr,"\n%s: left height %d, right height %d, difference %d, not balanced", tree_name_node->treeName, left, right, diff); 
  }

  
  
}

void queries(treeNameNode *my_root, int total_opertions) {
  // first element: operation type
  // second element: treeName Node
  // third element: item needed to do operation

  char first_element[MAXLEN];
  char second_element[MAXLEN];
  char third_element[MAXLEN];
  int reduce_amount;

  while (total_opertions != 0) {
    scanf("%s", first_element);

    if (strcmp(first_element, "search") == 0) {
      scanf("%s %s", second_element, third_element);

      // get the correct tree name root
      treeNameNode *tree_name_root =
          searchTreeNameRoot(my_root, second_element);

      if (tree_name_root == NULL) {
        printf("\n%s does not exist", second_element);
        fprintf(fptr,"\n%s does not exist", second_element);
      } else {
        itemNode *item =
            search_item_in_tree_name(tree_name_root->theTree, third_element);

        if (item != NULL) {
          printf("\n%d %s found in %s", item->count, item->name,
                 tree_name_root->treeName);
          fprintf(fptr, "\n%d %s found in %s",  item->count, item->name,
                 tree_name_root->treeName);
        } else {
          printf("\n%s not found in %s", third_element, second_element);
          fprintf(fptr, "\n%s not found in %s",  third_element, second_element);
        }
      }

    } 
    else if(strcmp(first_element, "item_before") == 0) {
      scanf("%s %s", second_element, third_element);

      treeNameNode *tree_name_root = searchTreeNameRoot(my_root, second_element);

      int count = item_before(tree_name_root->theTree, third_element);
      printf("\nitem before %s: %d",third_element, count);
      fprintf(fptr,"\nitem before %s: %d",third_element, count);
      
    }
    else if (strcmp(first_element, "count") == 0) {
      scanf("%s ", second_element); //animal

      treeNameNode* tree_name_root = searchTreeNameRoot(my_root, second_element);

      int cnt = count(tree_name_root->theTree);

      printf("\n%s count %d", second_element, cnt);
      fprintf(fptr,"\n%s count %d", second_element, cnt);
      
    }
    else if (strcmp(first_element, "delete") == 0) {
      //delete an item
      scanf("%s %s", second_element, third_element);

      //find the tree name root
      treeNameNode* tree_name_root = searchTreeNameRoot(my_root, second_element);

      tree_name_root->theTree = deletion(tree_name_root->theTree, third_element);

      itemNode *item2 = search_item_in_tree_name(tree_name_root->theTree, third_element);
      
      if(item2 == NULL){
        printf("\n%s deleted from %s", third_element, second_element);
        fprintf(fptr,"\n%s deleted from %s", third_element, second_element);
      }
      
    }  
    else if (strcmp(first_element, "reduce") == 0) {
      scanf("%s %s %d", second_element, third_element,&reduce_amount);

      treeNameNode* tree_name_root = searchTreeNameRoot(my_root, second_element);

      tree_name_root->theTree = reduce(tree_name_root->theTree, third_element, reduce_amount);
      
      printf("\n%s reduced ",third_element);
      fprintf(fptr,"\n%s reduced ",third_element);
    
      
    }
    else if (strcmp(first_element, "delete_name") == 0) {
      scanf("%s ", second_element); //animal

      //get the correct tree name root
      treeNameNode* tree_name_node = searchTreeNameRoot(my_root, second_element);

      //delete all of the item in the item tree
      deleteAllItems(tree_name_node->theTree);

      //delete treeName node in tree name tree
      my_root = deletion2(my_root, second_element);

      
      //check if deletion is successful
      treeNameNode* check_item = searchTreeNameRoot(my_root, second_element);
      
      if(check_item == NULL){
        printf("\n%s deleted ",second_element);
        fprintf(fptr,"\n%s deleted ",second_element);
      }
      
    }
    else if(strcmp(first_element,"height_balance") == 0){
      scanf("%s ", second_element);

      treeNameNode* tree_name_node = searchTreeNameRoot(my_root, second_element);
      
      heightBalance( tree_name_node,tree_name_node->theTree);
    }
       
    total_opertions--;
  }
}

int main() {

  atexit(report_mem_leak);
  
  int num_of_tree_names = 0;
  int total_items;
  int total_operations;

  treeNameNode *my_root = NULL;

  fptr = fopen("myout2.txt", "a");

  scanf("%d %d %d", &num_of_tree_names, &total_items, &total_operations);

  my_root = buildNameTree(my_root, num_of_tree_names);

  builItemTree(my_root, total_items);

  print_tree_name(my_root);

  traverse_in_traverse(my_root);

  queries(my_root, total_operations);

  freeTreeNames(my_root);

  fclose(fptr);

  return 0;
}