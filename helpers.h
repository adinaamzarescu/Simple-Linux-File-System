#ifndef HELPERS_H_
#define HELPERS_H_

#include "tree.h"

void lsAll(ListNode* node);
void lsFolder(TreeNode* currentNode);
void lsFile(TreeNode* currentNode);
void print_rec(TreeNode *treeNode);
TreeNode *findFolder(TreeNode* currentNode, char* token);
void tree_print(ListNode *target, int level, int *folder_nr, int *file_nr);
TreeNode *makeFileNode(TreeNode* currentNode, char* fileName);
TreeNode *makeFolderNode(TreeNode* currentNode, char* folderName);
int check_existance(TreeNode* currentNode, char* folderName);
List * create_new_list(TreeNode* currentNode, TreeNode *new_file);

void free_list(List *list);
void freeFile(TreeNode *file);
void freeFolder(TreeNode *folder);

#endif  // HELPERS_H_
