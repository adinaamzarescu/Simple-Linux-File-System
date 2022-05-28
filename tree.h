#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."
#define TOKEN_MAX_LEN 300

typedef struct FileContent FileContent;
typedef struct FolderContent FolderContent;
typedef struct TreeNode TreeNode;
typedef struct FileTree FileTree;
typedef struct ListNode ListNode;
typedef struct List List;

enum TreeNodeType {
    FILE_NODE,
    FOLDER_NODE
};

struct FileContent {
    char* text;
};

struct FolderContent {
    List* children;
};

struct TreeNode {
    TreeNode* parent;
    char* name;
    enum TreeNodeType type;
    void* content;
};

struct FileTree {
    TreeNode* root;
};

struct ListNode {
    TreeNode* info;
    ListNode* next;
};

struct List {
    ListNode* head;
};


void ls(TreeNode* current_node, char* arg);
void pwd(TreeNode* treeNode);
TreeNode* cd(TreeNode* current_node, char* path, int tree_cmd);
void tree(TreeNode* current_node, char* arg);
void mkdir(TreeNode* current_node, char* folder_name);
void rm(TreeNode* current_node, char* file_name);
void rmdir(TreeNode* current_node, char* folder_name);
void rmrec(TreeNode* current_node, char* resource_name);
void touch(TreeNode* current_node, char* file_name, char* file_content);
void cp(TreeNode* current_node, char* source, char* destination);
void mv(TreeNode* current_node, char* source, char* destination);
FileTree *create_file_tree();
void free_tree(FileTree *file_tree);

#endif  // TREE_H_
