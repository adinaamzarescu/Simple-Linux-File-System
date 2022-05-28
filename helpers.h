#ifndef HELPERS_H_
#define HELPERS_H_

#include "tree.h"

void ls_all(ListNode* node);
void ls_folder(TreeNode* current_node);
void ls_file(TreeNode* current_node);
void print_rec(TreeNode *tree_node);
TreeNode *find_folder(TreeNode* current_node, char* token);
void tree_print(ListNode *target, int level, int *folder_nr, int *file_nr);
TreeNode *make_file_node(TreeNode* current_node, char* file_name);
TreeNode *make_folder_node(TreeNode* current_node, char* folder_name);
int check_existance(TreeNode* current_node, char* folder_name);
List * create_new_list(TreeNode* current_node, TreeNode *new_file);

void free_list(List *list);
void free_file(TreeNode *file);
void free_folder(TreeNode *folder);

#endif  // HELPERS_H_
