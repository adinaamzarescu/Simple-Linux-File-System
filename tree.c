#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."



// ------------------------------------------------

// #define TREE_CMD_INDENT_SIZE 4
// #define NO_ARG ""
// #define PARENT_DIR ".."

// #define MAX 300

// typedef struct FileContent FileContent;
// typedef struct FolderContent FolderContent;
// typedef struct TreeNode TreeNode;
// typedef struct FileTree FileTree;
// typedef struct ListNode ListNode;
// typedef struct List List;

// enum TreeNodeType {
//     FILE_NODE,
//     FOLDER_NODE
// };

// struct FileContent {
//     char* text;
// };

// struct FolderContent {
//     List* children;
// };

// struct TreeNode {
//     TreeNode* parent;
//     char* name;
//     enum TreeNodeType type;
//     void* content;
// };

// struct FileTree {
//     TreeNode* root;
// };

// struct ListNode {
//     TreeNode* info;
//     ListNode* next;
// };

// struct List {
//     ListNode* head;
// };


// void ls(TreeNode* currentNode, char* arg);
// void pwd(TreeNode* treeNode);
// TreeNode* cd(TreeNode* currentNode, char* path);
// void tree(TreeNode* currentNode, char* arg);
// void mkdir(TreeNode* currentNode, char* folderName);
// void rm(TreeNode* currentNode, char* fileName);
// void rmdir(TreeNode* currentNode, char* folderName);
// void rmrec(TreeNode* currentNode, char* resourceName);
// void touch(TreeNode* currentNode, char* fileName, char* fileContent);
// void cp(TreeNode* currentNode, char* source, char* destination);
// void mv(TreeNode* currentNode, char* source, char* destination);
// FileTree createFileTree();
// void freeTree(FileTree fileTree);

// --------------------------------------------------


FileTree createFileTree(char* rootFolderName) {

    FileTree *new_file = malloc (sizeof(*new_file));
    DIE(!new_file, "Malloc failed");

    new_file->root = (struct TreeNode *) malloc(sizeof(struct TreeNode));
    DIE(!new_file->root, "Malloc failed");

    new_file->root->name = malloc(TOKEN_MAX_LEN);
    DIE(!new_file->root->name, "Malloc failed");
    
    memcpy(new_file->root->name, rootFolderName, strlen(rootFolderName) + 1);

    new_file->root->parent = NULL;
    new_file->root->type = 0;
    new_file->root->content = (struct FolderContent *)malloc(sizeof(struct FolderContent));
    DIE(!new_file->root->content, "Malloc failed");
    
    List* children = (List*)malloc(sizeof(List));
    children = (List *)new_file->root->content;

    children->head = NULL;

    // children->head = (ListNode *) malloc(sizeof(ListNode));
    // children->head->next = NULL;
    // children->head->info = NULL;

    return *new_file;
    
}

void freeFile (TreeNode *file) {

    if (!file)
        return;
    FileContent *cont = (FileContent *)file->content;

    free(file->name);
    free(cont);
    free(file);

}

void freeFolder (TreeNode *folder) {
    if (!folder)
        return;
    FolderContent *cont = (FolderContent *)folder->content;

    
    // In order to free all the folders
    ListNode *node = cont->children->head;
    TreeNode *prev = NULL;

    if (node)
        while (node->next) {
            prev = (TreeNode *) node;
            node = node->next;
            freeFolder(prev);
        }
    // 0 -> file
    // 1 -> folder
    int type = cont->children->head->info->type;
    TreeNode *directory = (TreeNode *)cont->children->head;
    if (type)
        freeFolder(directory);
    else
        freeFile(directory);
    
    free(folder->name);
    free(folder);
    
}

void freeTree(FileTree fileTree) {
    
    FileTree *tree = &fileTree;
    TreeNode *node = (TreeNode *) tree->root;

    freeFolder(node);
    node = NULL;
}

int findArg(TreeNode* currentNode, char *arg) {

}

void lsFolder (TreeNode* currentNode) {
    ls (currentNode, 0);
}

void lsFile (TreeNode* currentNode) {
    char *cnt = (char *) currentNode->content;
    printf("%s: %s\n", currentNode->name, cnt);
}

void ls(TreeNode* currentNode, char* arg) {
    List *tree = (List *) currentNode;
    ListNode *node = tree->head;
    ListNode *aux = tree->head;
    TreeNode *curr;

    while(node) {
        if (node->info) {
            if (*arg) {
                if (!strcmp(node->info->name, arg)) {
                    curr = (TreeNode *) node;
                    if (node->info->type) {
                        lsFolder(curr);
                    } else {
                        lsFile(curr);
                    }
                } else {
                node = node->next;
                }
            } else {
                while (aux && aux->info->name) {
                    printf("%s\n", aux->info->name);
                    aux = aux->next;
                }
                break;
            }
        }
    }
}


void pwd(TreeNode* treeNode) {
    // TODO
}


TreeNode* cd(TreeNode* currentNode, char* path) {
    // TODO
}


void tree(TreeNode* currentNode, char* arg) {
    // TODO
}

TreeNode *makeFileNode(TreeNode* currentNode, char* fileName) {
    TreeNode *new_node = malloc(sizeof(*new_node));

    new_node->name = malloc(TOKEN_MAX_LEN);
    memcpy(new_node->name, fileName, strlen(fileName) + 1);

    new_node->parent = currentNode;
    new_node->type = 0;
    new_node->content = malloc(TOKEN_MAX_LEN);

    return new_node;
}

TreeNode *makeFolderNode(TreeNode* currentNode, char* folderName) {
    TreeNode *new_node = malloc(sizeof(*new_node));

    new_node->name = malloc(TOKEN_MAX_LEN);
    memcpy(new_node->name, folderName, strlen(folderName) + 1);

    new_node->parent = currentNode;
    new_node->type = 1;
    new_node->content = (struct FolderContent *)malloc(sizeof(struct FolderContent));

    return new_node;
}

int check_existance (TreeNode* currentNode, char* folderName) {
    // This function will return 1 if there
    // is a folder with the same name
    // and 0 if not
    List *new_list = (List *) currentNode;
    ListNode *node = new_list->head;
    if(node) {
        if(!strcmp(node->info->name, folderName))
            return 1;
        while(node->next) {
            if(!strcmp(node->info->name, folderName))
                return 1;
            node = node->next;
        }
        if(!strcmp(node->info->name, folderName))
                return 1;
    }
    return 0;
}

void mkdir(TreeNode* currentNode, char* folderName) {
    TreeNode *new_dir = makeFolderNode(currentNode, folderName);

    List* new_list = malloc(sizeof(*new_list));
    new_list->head = (ListNode*)malloc(sizeof(ListNode));
    new_list->head->next = NULL;
    new_list->head->info = new_dir;
    new_list->head->info->parent = currentNode;

    List *old_list = (List *) currentNode;
    ListNode *node = (ListNode *) old_list->head;

    if(!node) {
        // If this is the first folder in the cuurent directory
        if(check_existance(currentNode, folderName)) {
            printf("mkdir: cannot create directory %s: File exists", folderName);
        } else {
            node = malloc (sizeof(* node));
            node->next = NULL;
            node->info = new_dir;
            old_list->head = node;
        }
    } else {
        if(check_existance(currentNode, folderName)) {
            printf("mkdir: cannot create directory %s: File exists", folderName);
        } else {
            while (node->next)
                // In order to add it at the end of the list
                node = node->next;
            // The loop will stop at the previous directory
            // So in order to add the new directory in the list of 
            // directories, the last directory's next has to be changed
            node->next = malloc(sizeof(*node->next));
            node->next->info = new_dir;
            memcpy(node->next->info->name, new_dir->name, strlen(new_dir->name) + 1);
            // node->next->info->parent = (TreeNode *) node;
            node->next->next = NULL;
        }
    }
}

void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    TreeNode *new_file = makeFileNode(currentNode, fileName);

    new_file->content = (struct FileContent *) malloc (sizeof (struct FileContent));
    // char *cont =
    memcpy((char *)new_file->content, fileContent, strlen(fileContent) + 1);

    List *old_list = (List *) currentNode;
    ListNode *node = (ListNode *) old_list->head;

    if(!node) {
        // If this is the first folder in the cuurent directory
        if(!check_existance(currentNode, fileName)) {
            node = malloc (sizeof(* node));
            node->next = NULL;
            node->info = new_file;
            old_list->head = node;
        }
    } else {
        if(!check_existance(currentNode, fileName)) {
            while (node->next)
                // In order to add it at the end of the list
                node = node->next;
            // The loop will stop at the previous file
            // So in order to add the new file in the list of 
            // files, the last file's next has to be changed
            node->next = (ListNode *)new_file;
            memcpy(node->next->info->name, new_file->name, strlen(new_file->name) + 1);
            memcpy(node->next->info->content, new_file->content, strlen(new_file->content) + 1);
            node->next->info = new_file;
            node->next->next = NULL;
        }
    }
}

void rmrec(TreeNode* currentNode, char* resourceName) {
    // TODO
}


void rm(TreeNode* currentNode, char* fileName) {
    // TODO
}


void rmdir(TreeNode* currentNode, char* folderName) {
    // TODO
}


void cp(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

// --------------------------------------------------------------


#define LINE_MAX_LEN 1000
#define TOKEN_MAX_LEN 300

#define LS "ls"
#define PWD "pwd"
#define TREE "tree"
#define CD "cd"
#define MKDIR "mkdir"
#define RMDIR "rmdir"
#define TOUCH "touch"
#define RM "rm"
#define RMREC "rmrec"
#define MV "mv"
#define CP "cp"

void execute_command(char *cmd, char *arg1, char *arg2) {
    printf("$ %s %s %s\n", cmd, arg1, arg2);
}

TreeNode* process_command(TreeNode* currentFolder,
        char cmd[3][TOKEN_MAX_LEN], int token_count) {
    execute_command(cmd[0], cmd[1], cmd[2]);
    if (!strcmp(cmd[0], LS)) {
        ls(currentFolder, cmd[1]);
    } else if (!strcmp(cmd[0], PWD)) {
        pwd(currentFolder);
    } else if (!strcmp(cmd[0], TREE)) {
        tree(currentFolder, cmd[1]);
    } else if (!strcmp(cmd[0], CD)) {
        currentFolder = cd(currentFolder, cmd[1]);
    } else if (!strcmp(cmd[0], MKDIR)) {
        mkdir(currentFolder, strdup(cmd[1]));
    } else if (!strcmp(cmd[0], RMDIR)) {
        rmdir(currentFolder, cmd[1]);
    } else if (!strcmp(cmd[0], RM)) {
        rm(currentFolder, cmd[1]);
    } else if (!strcmp(cmd[0], RMREC)) {
        rmrec(currentFolder, cmd[1]);
    } else if (!strcmp(cmd[0], TOUCH)) {
        touch(currentFolder, strdup(cmd[1]), strdup(cmd[2]));
    } else if (!strcmp(cmd[0], MV)) {
        mv(currentFolder, cmd[1], cmd[2]);
    } else if (!strcmp(cmd[0], CP)) {
        cp(currentFolder, cmd[1], cmd[2]);
    } else {
        printf("UNRECOGNIZED COMMAND!\n");
    }
    printf("\n");
    return currentFolder;
}

int main() {
    char line[LINE_MAX_LEN];
    char cmd[3][TOKEN_MAX_LEN];
    char *token;

    FileTree fileTree = createFileTree(strdup("root"));
    TreeNode* currentFolder = fileTree.root;

    // freeTree(fileTree);

    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strlen(line)-1] = 0;

        cmd[0][0] = cmd[1][0] = cmd[2][0] = 0;

        int token_idx = 0;
        token = strtok(line, " ");
        while (token) {
            strcpy(cmd[token_idx], token);
            ++token_idx;

            token = strtok(NULL, " ");
        }
        currentFolder = process_command(currentFolder, cmd, token_idx);
    }

    freeTree(fileTree);

    return 0;
}

// --------------------------------------------------------------
