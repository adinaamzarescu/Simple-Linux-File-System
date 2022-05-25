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


FileTree *createFileTree(char* rootFolderName) {
    FileTree *new_file = (FileTree *) malloc (sizeof(FileTree));
    DIE(!new_file, "Malloc failed");

    new_file->root = (struct TreeNode *) malloc(sizeof(struct TreeNode));
    DIE(!new_file->root, "Malloc failed");

    new_file->root->name = malloc(TOKEN_MAX_LEN);
    DIE(!new_file->root->name, "Malloc failed");

    memcpy(new_file->root->name, rootFolderName, strlen(rootFolderName) + 1);

    new_file->root->parent = NULL;
    new_file->root->type = 1;
    new_file->root->content = NULL;

    free(rootFolderName);
    return new_file;
}

void free_list(List *list) {
    if (!list)
        return;
    if (list->head) {
        free(list->head);
        list->head = NULL;
    }
    // if(list->head->next) {
    //     free(list->head->next);
    //     list->head->next;
    // }
    if (list) {
        free(list);
        list = NULL;
    }
}

void freeFile(TreeNode *file) {
    if (!file)
        return;
    List *old_list = NULL;

    if (file->parent)
        old_list = (List *)file->parent->content;

    if (old_list) {
        if (old_list->head && (!old_list->head->next)) {
            if (!strcmp(old_list->head->info->name, file->name)) {
                // Clearing the old list
                free_list(old_list);
            }
        }
    }

    if (file->name) {
        free(file->name);
    file->name = NULL;
    }

    if (file->content) {
        free(file->content);
        file->content = NULL;
    }

    if (file) {
        free(file);
        file = NULL;
    }
}

void freeFolder(TreeNode *folder) {
    if (!folder)
        return;
    // Old_list will be the content of the parent
    List *old_list = NULL;
    if (folder->parent)
        old_list = (List *)folder->parent->content;

    // New_list will be the content of the current folder
    List *new_list = NULL;
    if (folder->type)
        new_list = (List *)folder->content;


    ListNode *prev = NULL;
    ListNode *node = NULL;
    // In order to free all the folders
    // If the current folder has a content
    if (new_list) {
        // node will be the first file in the content
        node = new_list->head;
        if (node) {
            // Going step by step in the list
            // removing them at each step
            if (node->next) {
                while (node->next) {
                    prev = node;
                    node = node->next;
                    // Checking the type of the file
                    if (prev->info->type) {
                        freeFolder(prev->info);
                    } else {
                        freeFile(prev->info);
                    }
                }
                // This loop will end at the previous node
                // For the last node
                if (node->info->type) {
                    freeFolder(node->info);
                } else {
                    freeFile(node->info);
                }
                node->info = NULL;
                prev = NULL;
            } else {
                // If this is the only file in the list
                if (node->info->type) {
                    freeFolder(node->info);
                } else {
                    freeFile(node->info);
                }
            }
            if (node) {
                node = NULL;
            }
        }
        // The new list will become null
        // free(new_list);
        new_list = NULL;
    }

    // Now the folder is empty
    // But just to be safe
    if (folder->content)
        folder->content = NULL;

    // Testing if the folder was the only
    // one in the content of the parent
    if (old_list) {
        if (old_list->head && (!old_list->head->next)) {
            if (!strcmp(old_list->head->info->name, folder->name)) {
                // Clearing the old list
                free(old_list->head);
                old_list->head = NULL;
                free(old_list);
                old_list = NULL;
            }
        }
    }
    // Now clearing the folder
    free(folder->name);
    folder->name = NULL;

    free(folder);
    folder = NULL;
}

void freeTree(FileTree *fileTree) {
    FileTree *new_file = fileTree;
    TreeNode *node = new_file->root;

    freeFolder(node);
    node = NULL;
    free(new_file);
    // tree = NULL;
    fileTree = NULL;
    new_file = NULL;
}

void lsAll(ListNode* node) {
    if (node && node->next) {
        lsAll(node->next);
        printf("%s\n", node->info->name);
    } else if (!node) {
        return;
    } else {
        printf("%s\n", node->info->name);
    }
}

void lsFolder(TreeNode* currentNode) {
    List *list = currentNode->content;
    ListNode *node = list->head;
    lsAll(node);
}

void lsFile(TreeNode* currentNode) {
    char *cnt = (char *) currentNode->content;
    printf("%s: %s\n", currentNode->name, cnt);
}

void ls(TreeNode* currentNode, char* arg) {
    int found = 0;
    if (!currentNode)
        return;
    List *tree = (List *) currentNode->content;
    if (!tree)
        return;
    if (!tree->head)
        return;
    ListNode *node = tree->head;
    TreeNode *curr;

    while (node) {
        if (node->info) {
            if (*arg) {
                if (!strcmp(node->info->name, arg)) {
                    found = 1;
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
                lsAll(node);
                break;
            }
        }
    }

    if (!found && *arg) {
        printf("ls: cannot access '%s': No such file or directory\n", arg);
    }
}

// Function that prints the path
void print_rec(TreeNode *treeNode) {
    if (treeNode->parent) {
        print_rec(treeNode->parent);
        printf("%s/", treeNode->name);
    } else {
        return;
    }
}

void pwd(TreeNode* treeNode) {
    if (!strcmp(treeNode->name, "root")) {
        printf("root");
        return;
    } else {
        printf("root/");
    }
    print_rec(treeNode->parent);
    // Because the last folder
    // shouldn't end with "/"
    printf("%s", treeNode->name);
}

TreeNode *findFolder(TreeNode* currentNode, char* token) {
    List *tree_list = (List *) currentNode->parent->content;
    ListNode *node = tree_list->head;
    if (node) {
        if (!strcmp(node->info->name, token))
            return node->info;
        while (node->next) {
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
    // Special case if the current node is the root
    if (!strcmp(currentNode->name, "root")) {
        if (!strncmp(path, PARENT_DIR, 2)) {
            printf("cd: no such file or directory: %s\n", path);
            return currentNode;
        }
    }
    TreeNode *tree_node = currentNode;
    List *tree_list = (List *) currentNode->content;
    char *new_path = path;

    // If the content of the folder is empty
    if (!tree_list) {
        // Checking if the path is PARENT_DIR
        if (!strncmp(path, PARENT_DIR, 2)) {
            // path + 3 because besides ..
            // there will also be the '/' char
            new_path = path + 3;
            if (!strcmp(new_path, NO_ARG))
                tree_node = currentNode->parent;
            else
                // printf("cd: no such file or directory: %s\n", path);
                tree_node = cd(currentNode->parent, new_path);
        } else {
            printf("cd: no such file or directory: %s\n", path);
        }
        return tree_node;
    } else {
        // If the folder is not empty
        ListNode *node = tree_list->head;
        TreeNode *current_node = node->info;
        // Infinite loop in order to check each node
        // of the list
        while (1) {
            // If the node is a folder
            if (node->info->type) {
                char *token;
                token = strtok(path, "/");
                while (token) {
                    if (!strcmp(token, PARENT_DIR)) {
                        tree_node = currentNode->parent;
                    } else {
                        tree_node = findFolder(current_node, token);
                        if (!tree_node) {
                            printf("cd: no such file or directory: %s\n", path);
                            tree_node = currentNode;
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
                // If it's a file, not a folder
                if (node->next) {
                    node = node->next;
                } else {
                    if (!strncmp(path, PARENT_DIR, 2)) {
                        // path + 3 because besides ..
                        // there will also be the '/' char
                        new_path = path + 3;
                        if (!strcmp(new_path, NO_ARG)) {
                            tree_node = currentNode->parent;
                        } else {
                            tree_node = cd(currentNode->parent, new_path);
                        }
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
    // TODO(stuff) :stuff
}

TreeNode *makeFileNode(TreeNode* currentNode, char* fileName) {
    TreeNode *new_node = malloc(sizeof(*new_node));

    new_node->name = malloc(TOKEN_MAX_LEN);
    memcpy(new_node->name, fileName, strlen(fileName) + 1);

    new_node->parent = currentNode;
    new_node->type = 0;
    // new_node->content = calloc(TOKEN_MAX_LEN, sizeof(char *));
    new_node->content = NULL;

    return new_node;
}

TreeNode *makeFolderNode(TreeNode* currentNode, char* folderName) {
    TreeNode *new_node = malloc(sizeof(*new_node));

    new_node->name = malloc(TOKEN_MAX_LEN);
    memcpy(new_node->name, folderName, strlen(folderName) + 1);

    new_node->parent = currentNode;
    new_node->type = 1;
    new_node->content = NULL;

    return new_node;
}

int check_existance(TreeNode* currentNode, char* folderName) {
    // This function will return 1 if there
    // is a folder with the same name
    // and 0 if not
    List *new_list = (List *) currentNode->content;
    if (new_list->head) {
        ListNode *node = new_list->head;
        TreeNode *current_node = node->info;
        if (!strcmp(current_node->name, folderName))
            return 1;
        while (node->next) {
            node = node->next;
            if (!strcmp(current_node->name, folderName))
                return 1;
            current_node = node->info;
        }
        if (!strcmp(current_node->name, folderName))
                return 1;
    }
    return 0;
}

List * create_new_list(TreeNode* currentNode, TreeNode *new_file) {
    List* new_list = malloc(sizeof(*new_list));
    new_list->head = (ListNode*)malloc(sizeof(ListNode));
    new_list->head->next = NULL;
    new_list->head->info = new_file;
    new_list->head->info->parent = currentNode;

    return new_list;
}

void mkdir(TreeNode* currentNode, char* folderName) {
    int ok = 1;
    TreeNode *new_dir = makeFolderNode(currentNode, folderName);

    List *old_list = (List *) currentNode->content;
    ListNode *node;
    if (old_list)
        node = old_list->head;

    if (!old_list) {
        List *old_list = create_new_list(currentNode, new_dir);
        // If this is the first folder in the cuurent directory
        node = old_list->head;
        node->next = NULL;
        node->info = new_dir;
        currentNode->content = old_list;
    } else {
        if (check_existance(currentNode, folderName)) {
            ok = 0;
            printf("mkdir: cannot create directory '%s': File exists",
                    folderName);
        } else {
            while (node->next) {
                // In order to add it at the end of the list
                node = node->next;
            }
            // The loop will stop at the previous directory
            // So in order to add the new directory in the list of
            // directories, the last directory's next has to be changed
            node->next = (ListNode *)malloc(sizeof(ListNode));
            node->next->info = new_dir;
            memcpy(node->next->info->name, new_dir->name,
                   strlen(new_dir->name) + 1);
            // node->next->info->parent = (TreeNode *) node;
            node->next->next = NULL;
            node->next->info->content = NULL;
        }
    }

    free(folderName);
}

void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    // Creating new node
    TreeNode *new_file = makeFileNode(currentNode, fileName);
    // Allocating memory for the content
    new_file->content = calloc (TOKEN_MAX_LEN, sizeof (char));
    // Copying the content
    memcpy(new_file->content, fileContent, strlen(fileContent) + 1);

    // Old list will be the content of the parent
    List *old_list = (List *) currentNode->content;
    ListNode *node = NULL;

    // If the parent has other files
    if (old_list)
        node = old_list->head;

    // If the parent's content is empty
    if (!old_list) {
        List *old_list = create_new_list(currentNode, new_file);
        memcpy((char *)old_list->head->info->content, fileContent,
                strlen(fileContent) + 1);

        // If this is the first folder in the cuurent directory
        node = old_list->head;
        node->next = NULL;
        node->info = new_file;
        currentNode->content = old_list;
    } else {
        if (!check_existance(currentNode, fileName)) {
            while (node->next)
                // In order to add it at the end of the list
                node = node->next;
            // The loop will stop at the previous file
            // So in order to add the new file in the list of
            // files, the last file's next has to be changed
            node->next = (ListNode *)malloc(sizeof(ListNode));
            node->next->info = new_file;
            memcpy(node->next->info->name, new_file->name,
                   strlen(new_file->name) + 1);
            memcpy(node->next->info->content, new_file->content,
                   strlen(new_file->content) + 1);
            node->next->next = NULL;
        }
    }
    free(fileName);
    free(fileContent);
}

void rmrec(TreeNode* currentNode, char* resourceName) {
    List *tree_list = (List *) currentNode->content;
    ListNode *node = tree_list->head;
    int is_dir = 0;
    TreeNode *file = node->info;
    ListNode *prev = NULL;
    ListNode *target = node;
    int found = 0;

    if (node) {
        while (node) {
            if (!strcmp(node->info->name, resourceName)) {
                // There is a file with the given name
                found = 1;
                // Searching for name in the list
                if (node->info->type) {
                    // Checking if it is a file, not a directory
                    is_dir = 1;
                }
                target = node;
                file = node->info;
                break;
            }
            prev = node;
            node = node->next;
        }
    }
    if (!found) {
        printf("rmrec: failed to remove '%s': No such file or directory\n",
                resourceName);
        return;
    }
    // First linking the previous node to the next one
    if (!prev) {
        if (target->next) {
            tree_list->head = target->next;
        } else {
            free_list(tree_list);
            tree_list = NULL;
            currentNode->content = NULL;
        }
    } else {
        prev->next = target->next;
    }

    if (is_dir) {
        // Then deleting the node
        freeFolder(file);
    } else {
        // Then deleting the node
        freeFile(file);
    }

    file = NULL;
    if (node) {
        // free(node);
        node = NULL;
    }
    if (tree_list)
        if (!tree_list->head)
            tree_list = NULL;
    target = NULL;
}


void rm(TreeNode* currentNode, char* fileName) {
    List *tree_list = (List *) currentNode->content;
    ListNode *node = (ListNode *) tree_list->head;
    int found = 0;
    TreeNode *file;
    ListNode *prev = NULL;

    if (node) {
        file = node->info;
        while (node) {
            if (!strcmp(node->info->name, fileName)) {
                // Searching for name in the list
                if (!node->info->type) {
                    // Checking if it is a file, not a directory
                    found = 1;
                    file = node->info;
                } else {
                    printf("rm: cannot remove '%s': Is a directory", fileName);
                    return;
                }
                break;
            }
            prev = node;
            node = node->next;
        }
    }

    if (found) {
        rmrec(currentNode, fileName);
        if (tree_list)
            tree_list = NULL;
        if (node)
            node = NULL;
        if (file)
            file = NULL;
    } else {
        printf("rm: failed to remove '%s': No such file or directory",
                fileName);
        return;
    }
}


void rmdir(TreeNode* currentNode, char* folderName) {
    List *tree_list = (List *) currentNode->content;
    if (!tree_list) {
        printf("rmdir: failed to remove '%s': No such file or directory",
                folderName);
        return;
    }
    ListNode *node = (ListNode *) tree_list->head;
    int found = 0;
    TreeNode *folder = node->info;
    ListNode *prev = NULL;

    if (node) {
        while (node) {
            if (!strcmp(node->info->name, folderName)) {
                // Searching for name in the list
                if (node->info->type) {
                    // Checking if it is a folder, not a file
                    found = 1;
                    folder = node->info;
                } else {
                    printf("rmdir: failed to remove '%s': Not a directory\n",
                            folderName);
                    return;
                }
                break;
            }
            prev = node;
            node = node->next;
        }
    }
    if (found) {
        List *new_list = (List *)folder->content;
        if (!new_list) {
            // First linking the previous node to the next one
            rmrec(currentNode, folderName);
            if (tree_list)
                tree_list = NULL;
            if (node)
                node = NULL;
            if (folder)
                folder = NULL;
        } else {
            printf("rmdir: failed to remove '%s': Directory not empty",
                    folderName);
            return;
        }
    } else {
        printf("rmdir: failed to remove '%s': No such file or directory",
                folderName);
        return;
    }
}


void cp(TreeNode* currentNode, char* source, char* destination) {
    // TODO(stuff) : stuff
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO(stuff) : stuff
}

// --------------------------------------------------------------


// #define LINE_MAX_LEN 1000
// #define TOKEN_MAX_LEN 300

// #define LS "ls"
// #define PWD "pwd"
// #define TREE "tree"
// #define CD "cd"
// #define MKDIR "mkdir"
// #define RMDIR "rmdir"
// #define TOUCH "touch"
// #define RM "rm"
// #define RMREC "rmrec"
// #define MV "mv"
// #define CP "cp"

// void execute_command(char *cmd, char *arg1, char *arg2) {
//     printf("$ %s %s %s\n", cmd, arg1, arg2);
// }

// TreeNode* process_command(TreeNode* currentFolder,
//         char cmd[3][TOKEN_MAX_LEN], int token_count) {
//     execute_command(cmd[0], cmd[1], cmd[2]);
//     if (!strcmp(cmd[0], LS)) {
//         ls(currentFolder, cmd[1]);
//     } else if (!strcmp(cmd[0], PWD)) {
//         pwd(currentFolder);
//     } else if (!strcmp(cmd[0], TREE)) {
//         tree(currentFolder, cmd[1]);
//     } else if (!strcmp(cmd[0], CD)) {
//         currentFolder = cd(currentFolder, cmd[1]);
//     } else if (!strcmp(cmd[0], MKDIR)) {
//         mkdir(currentFolder, strdup(cmd[1]));
//     } else if (!strcmp(cmd[0], RMDIR)) {
//         rmdir(currentFolder, cmd[1]);
//     } else if (!strcmp(cmd[0], RM)) {
//         rm(currentFolder, cmd[1]);
//     } else if (!strcmp(cmd[0], RMREC)) {
//         rmrec(currentFolder, cmd[1]);
//     } else if (!strcmp(cmd[0], TOUCH)) {
//         touch(currentFolder, strdup(cmd[1]), strdup(cmd[2]));
//     } else if (!strcmp(cmd[0], MV)) {
//         mv(currentFolder, cmd[1], cmd[2]);
//     } else if (!strcmp(cmd[0], CP)) {
//         cp(currentFolder, cmd[1], cmd[2]);
//     } else {
//         printf("UNRECOGNIZED COMMAND!\n");
//     }
//     printf("\n");
//     return currentFolder;
// }

// int main() {
//     char line[LINE_MAX_LEN];
//     char cmd[3][TOKEN_MAX_LEN];
//     char *token;

//     FileTree *fileTree = createFileTree(strdup("root"));
//     TreeNode* currentFolder = fileTree->root;

//     // freeTree(fileTree);
//     // fileTree.root = NULL;
//     // currentFolder = NULL;
//     // free(&fileTree);
//     // fileTree = NULL;

//     while (fgets(line, sizeof(line), stdin) != NULL) {
//         line[strlen(line)-1] = 0;

//         cmd[0][0] = cmd[1][0] = cmd[2][0] = 0;

//         int token_idx = 0;
//         token = strtok(line, " ");
//         while (token) {
//             strcpy(cmd[token_idx], token);
//             ++token_idx;

//             token = strtok(NULL, " ");
//         }
//         currentFolder = process_command(currentFolder, cmd, token_idx);
//     }

//     freeTree(fileTree);
//     fileTree = NULL;
//     // fileTree->root = NULL;
//     // currentFolder = NULL;

//     return 0;
// }


// --------------------------------------------------------------
