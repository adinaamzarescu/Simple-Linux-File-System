#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."



// ------------------------------------------------

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
    new_file->root->content = NULL;
    // DIE(!new_file->root->content, "Malloc failed");
    
    // List* children = (List*)malloc(sizeof(List));
    // children = (List *)new_file->root->content;

    // children->head = NULL;

    // children->head = (ListNode *) malloc(sizeof(ListNode));
    // children->head->next = NULL;
    // children->head->info = NULL;

    return *new_file;
    
}

void freeFile (TreeNode *file) {

    if (!file)
        return;
    char *cont = (char *)file->content;

    free(file->name);
    file->name = NULL;
    free(cont);
    cont = NULL;
    free(file);
    file = NULL;

}

void freeFolder (TreeNode *folder) {
    if (!folder)
        return;
    FolderContent *cont = (FolderContent *)folder->content;

    ListNode *node;
    // In order to free all the folders
    if (cont && cont->children) {
        node = cont->children->head;
        TreeNode *prev = NULL;

        if (node) {
            if (node->next)
                while (node->next) {
                    prev = (TreeNode *) node;
                    node = node->next;
                    freeFolder(prev);
                }
        }
    // 0 -> file
    // 1 -> folder
    int type = cont->children->head->info->type;
    TreeNode *directory = (TreeNode *)cont->children->head;
    if (type)
        freeFolder(directory);
    else
        freeFile(directory);
    }

    if (folder->content) {
        free(folder->content);
        folder->content = NULL;
    }

    free(folder->name);
    folder->name = NULL;
    free(folder);
    folder = NULL;

    // free(cont);
    // cont = NULL;
    // free(node);
    // node = NULL;
    
}

void freeTree(FileTree fileTree) {
    
    FileTree *tree = &fileTree;
    TreeNode *node = (TreeNode *) tree->root;

    freeFolder(node);
    node = NULL;
    tree = NULL;
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
    List *tree = (List *) currentNode->content;
    if (!tree)
        return;
    ListNode *node = tree->head;
    ListNode *aux = tree->head;
    TreeNode *curr;

    while(node) {
        if (node->info) {
            if (*arg) {
                if (!strcmp(node->info->name, arg)) {
                    curr = node->info;
                    if (node->info->type) {
                        lsFolder(curr);
                        break;
                    } else {
                        lsFile(curr);
                        break;
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

TreeNode *findFolder(TreeNode* currentNode, char* token) {
    List *tree_list = (List *) currentNode->parent->content;
    ListNode *node = tree_list->head;
    if (node) {
        if (!strcmp(node->info->name, token))
            return node->info; 
        while(node->next) {
            if (!strcmp(node->info->name, token))
                return node->info;
            node = node->next;
        }
        if (!strcmp(node->info->name, token))
                return node->info;
        }
    return NULL;
}

TreeNode* cd(TreeNode* currentNode, char* path) {
    TreeNode *tree_node = currentNode;
    List *tree_list = (List *) currentNode->content;

    if (!tree_list) {
        char back[3] = "..";
        char *new_path;
        if (!strncmp(back, path, 2)) {
            // path + 3 because besides ..
            // there will also be the '/' char
            memcpy(new_path, path + 3, strlen(path + 3) + 1);
            if (!strcmp(new_path, ""))
                tree_node = currentNode->parent;
            else
                tree_node = cd (currentNode->parent, new_path);
        } else {
            printf("cd: no such file or directory: %s\n", path);
        }
        return tree_node;
    } else {
        ListNode *node = tree_list->head;
        TreeNode *current_node = node->info;
        while (1) {
            if (node->info->type) {
                char *token;
                token = strtok(path, "/");
                while (token) {
                    if (!strcmp(token, "..")) {
                        tree_node = current_node->parent;
                    } else {
                        // if (!strcmp(current_node->name, token))
                        //     tree_node = current_node;
                        tree_node = findFolder(current_node, token);
                        if (!tree_node) {
                            printf("cd: no such file or directory: %s\n", path);
                            tree_node = current_node;
                            break;
                        }
                        if (node->next)
                            current_node = node->next->info;
                        else
                            break;
                        
                    }
                    // tree_node = NULL;
                    token = strtok(NULL, "/");
                }
                break;
            } else {
                if (node->next)
                    node = node->next;
                else {
                    char back[3] = "..";
                    char *new_path;
                    if (!strncmp(back, path, 2)) {
                        // path + 3 because besides ..
                        // there will also be the '/' char
                        memcpy(new_path, path + 3, strlen(path + 3) + 1);
                        if (!strcmp(new_path, ""))
                            tree_node = currentNode->parent;
                        else
                            tree_node = cd (currentNode->parent, new_path);
                    } else {
                        printf("cd: no such file or directory: %s\n", path);
                    }
                    return tree_node;
                }
            }
        }
    }

    return tree_node;
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
    List *new_list = (List *) currentNode->content;
    if(new_list->head) {
        ListNode *node = new_list->head;
        TreeNode *current_node = node->info;
        if(!strcmp(current_node->name, folderName))
            return 1;
        while(node->next) {
            node = node->next;
            if(!strcmp(current_node->name, folderName))
                return 1;
            current_node = node->info;
        }
        if(!strcmp(current_node->name, folderName))
                return 1;
    }
    return 0;
}

// List *createNewList(TreeNode* currentNode, TreeNode *new_file) {
//     List* new_list = malloc(sizeof(*new_list));
//     new_list->head = (ListNode*)malloc(sizeof(ListNode));
//     new_list->head->next = NULL;
//     new_list->head->info = new_file;
//     new_list->head->info->parent = currentNode;
//     new_list->head->info->content = NULL;
    
// }

void mkdir(TreeNode* currentNode, char* folderName) {
    int ok = 1;
    TreeNode *new_dir = makeFolderNode(currentNode, folderName);

    List *old_list = (List *) currentNode->content;
    ListNode *node;
    if (old_list)
     node = old_list->head;

    if(!old_list) {
        // Create a new list in which the head is the new folder
        List* new_list = malloc(sizeof(*new_list));
        new_list->head = (ListNode*)malloc(sizeof(ListNode));
        new_list->head->next = NULL;
        new_list->head->info = new_dir;
        new_list->head->info->parent = currentNode;
        new_list->head->info->content = NULL;

        // If this is the first folder in the cuurent directory
        node = malloc (sizeof(* node));
        node->next = NULL;
        node->info = new_dir;
        currentNode->content = (List *) malloc (sizeof(List));
        old_list = currentNode->content;
        old_list->head = (ListNode*)malloc(sizeof(ListNode));
        old_list->head = node;
    } else {
        if(check_existance(currentNode, folderName)) {
            ok = 0;
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
            node->next->info->content = NULL;
        }
    }

}

void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    TreeNode *new_file = makeFileNode(currentNode, fileName);

    new_file->content = (char *) malloc (sizeof (char) * TOKEN_MAX_LEN);
    char *cont = new_file->content;
    memcpy(cont, fileContent, strlen(fileContent) + 1);
    // printf("%s\n", cont);
    // printf("%s\n", (char *)new_file->content);

    List *old_list = (List *) currentNode->content;
    ListNode *node;
    if (old_list)
     node = old_list->head;

    if(!old_list) {

        List* new_list = malloc(sizeof(*new_list));
        new_list->head = (ListNode*)malloc(sizeof(ListNode));
        new_list->head->next = NULL;
        new_list->head->info = new_file;
        new_list->head->info->parent = currentNode;
        new_list->head->info->content = malloc(TOKEN_MAX_LEN);
        memcpy((char *)new_list->head->info->content, fileContent, strlen(fileContent) + 1);

        // If this is the first folder in the cuurent directory
        node = malloc (sizeof(* node));
        node->next = NULL;
        node->info = new_file;
        currentNode->content = (List *) malloc (sizeof(List));
        old_list = currentNode->content;
        old_list->head = (ListNode*)malloc(sizeof(ListNode));
        old_list->head = node;
    } else {
        if(!check_existance(currentNode, fileName)) {
            while (node->next)
                // In order to add it at the end of the list
                node = node->next;
            // The loop will stop at the previous file
            // So in order to add the new file in the list of 
            // files, the last file's next has to be changed
            node->next = malloc(sizeof(*node->next));
            node->next->info = new_file;
            memcpy(node->next->info->name, new_file->name, strlen(new_file->name) + 1);
            memcpy(node->next->info->content, new_file->content, strlen(new_file->content) + 1);
            node->next->next = NULL;
        }
    }
}

void rmrec(TreeNode* currentNode, char* resourceName) {
    List *tree_list = (List *) currentNode;
    ListNode *node = (ListNode *) tree_list->head;
    int is_dir = 0;
    TreeNode *file = node->info;
    ListNode *prev = node;

    if(node) {
        while(node) {
            if(!strcmp(node->info->name, resourceName)) {
                // Searching for name in the list
                if (node->info->type) {
                    // Checking if it is a file, not a directory
                    is_dir = 1;
                }
                file = node->info;
                break;
            }
            prev = node;
            node = node->next;
        }
    }
    // First linking the previous node to the next one
    prev->next = node->next;
    if (is_dir) {
        // Then deleting the node
        freeFolder(file);  
    }
    else {
        // Then deleting the node
        freeFile(file);
    }
    file = NULL;
}


void rm(TreeNode* currentNode, char* fileName) {
    List *tree_list = (List *) currentNode;
    ListNode *node = (ListNode *) tree_list->head;
    int found = 0;
    TreeNode *file = node->info;
    ListNode *prev = node;

    if(node) {
        while(node) {
            if(!strcmp(node->info->name, fileName)) {
                // Searching for name in the list
                if (!node->info->type) {
                    // Checking if it is a file, not a directory
                    found = 1;
                    file = node->info;
                } else {
                    printf("rm: cannot remove '%s': Is a directory", fileName);
                }
                break;
            }
            prev = node;
            node = node->next;
        }
    }
    if (found) {
        // First linking the previous node to the next one
        prev->next = node->next;
        // Then deleting the node
        freeFile(file);
        file = NULL;
    }
    else {
        printf("rm: failed to remove '%s': No such file or directory", fileName);
    }
}


void rmdir(TreeNode* currentNode, char* folderName) {
    List *tree_list = (List *) currentNode->content;
    if (!tree_list) {
        printf("rm: failed to remove '%s': No such file or directory", folderName);
        return;
    }
    ListNode *node = (ListNode *) tree_list->head;
    int found = 0;
    TreeNode *folder = node->info;
    ListNode *prev = NULL;

    if(node) {
        while(node) {
            if(!strcmp(node->info->name, folderName)) {
                // Searching for name in the list
                if (node->info->type) {
                    // Checking if it is a folder, not a file
                    found = 1;
                    folder = node->info;
                } else {
                    printf("rm: cannot remove '%s': Is a directory", folderName);
                }
                break;
            }
            prev = node;
            node = node->next;
        }
    }
    if (found) {
        List *new_list = (List *)folder->content;
        if(!new_list){
            // First linking the previous node to the next one
            if (!prev) {
                tree_list->head = node->next;
            } else {
                prev->next = node->next;
            }
            // Then deleting the node
            freeFolder(folder);
            folder = NULL;
        } else {
            printf("rmdir: failed to remove '%s': Directory not empty", folderName);
        }
    }
    else {
        printf("rm: failed to remove '%s': No such file or directory", folderName);
    }
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
