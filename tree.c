#include "tree.h"
#include "helpers.h"

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

TreeNode* cd(TreeNode* currentNode, char* path, int tree_cmd) {
    // Special case if the current node is the root
    if (!strcmp(currentNode->name, "root")) {
        if (!strncmp(path, PARENT_DIR, 2)) {
            if (!tree_cmd)
                printf("cd: no such file or directory: %s\n", path);
            else
                return NULL;
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
                tree_node = cd(currentNode->parent, new_path, 0);
        } else {
            if (!tree_cmd)
                printf("cd: no such file or directory: %s\n", path);
            else
                return NULL;
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
                        tree_node = findFolder(current_node->parent, token);
                        if (!tree_node) {
                            if (!tree_cmd)
                                printf("cd: no such file or directory: %s\n",
                                        path);
                            else
                                return NULL;

                            tree_node = currentNode;
                            break;
                        } else if (!tree_node->type) {
                            tree_node = currentNode;
                        }
                        // if (node->next)
                        //     current_node = node->next->info;
                        // else {
                            List *aux = (List *)node->info->content;
                            if (aux && aux->head) {
                                if (aux->head->info->type)
                                    current_node = aux->head->info;
                            // }
                        }
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
                            tree_node = cd(currentNode->parent, new_path, 0);
                        }
                    } else {
                        if (!tree_cmd)
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
    int folder_nr = 0;
    int file_nr = 0;
    List *tree_list = currentNode->content;
    ListNode *node = NULL;
    // ListNode *prev = NULL;
    if (tree_list)
        node = tree_list->head;
    else
        return;
    if (!strcmp(arg, NO_ARG)) {
        // If there is no path
        tree_print(node, 0, &folder_nr, &file_nr);
        // printf("%s\n", node->info->name);
        printf("\n%d directories, %d files\n", folder_nr, file_nr);
    } else {
        // Copying the path
        char *new_path = arg;
        TreeNode *tree_node = cd(currentNode, new_path, 1);

        if (!tree_node) {
            printf("%s [error opening dir]\n", arg);
            printf("\n0 directories, 0 files\n");
        } else {
            if (!strcmp(tree_node->name, currentNode->name)) {
                if (tree_node->type) {
                    printf("%s [error opening dir]\n", arg);
                    printf("\n0 directories, 0 files\n");
                } else {
                    printf("<path> [error opening dir]\n");
                    printf("\n0 directories, 0 files\n");
                }
                return;
            } else {
                tree(tree_node, NO_ARG);
            }
        }
    }
}

void mkdir(TreeNode* currentNode, char* folderName) {
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
