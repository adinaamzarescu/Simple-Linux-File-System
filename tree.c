#include "tree.h"
#include "helpers.h"

FileTree *create_file_tree(char* root_folder_name) {
    // Allocating memory for the new file
    FileTree *new_file = (FileTree *) malloc(sizeof(FileTree));
    DIE(!new_file, "Malloc failed");

    new_file->root = (struct TreeNode *) malloc(sizeof(struct TreeNode));
    DIE(!new_file->root, "Malloc failed");

    new_file->root->name = malloc(TOKEN_MAX_LEN);
    DIE(!new_file->root->name, "Malloc failed");

    // Initialization
    memcpy(new_file->root->name, root_folder_name,
           strlen(root_folder_name) + 1);

    new_file->root->parent = NULL;
    new_file->root->type = 1;
    new_file->root->content = NULL;

    // For the strdup allocation
    free(root_folder_name);
    return new_file;
}

void free_tree(FileTree *file_tree) {
    FileTree *new_file = file_tree;
    TreeNode *node = new_file->root;

    // Calling the function with root
    free_folder(node);
    node = NULL;
    free(new_file);
    file_tree = NULL;
    new_file = NULL;
}

void ls(TreeNode* current_node, char* arg) {
    // Used to check if arg is found
    int found = 0;
    if (!current_node)
        return;
    List *tree = (List *) current_node->content;

    if (!tree)
        return;
    if (!tree->head)
        return;

    ListNode *node = tree->head;
    TreeNode *curr;

    while (node) {
        if (node->info) {
            // If the function was called with an argument
            if (*arg) {
                if (!strcmp(node->info->name, arg)) {
                    found = 1;
                    curr = node->info;
                    // Depending if it's a file or a directory
                    if (node->info->type) {
                        ls_folder(curr);
                        break;
                    } else {
                        ls_file(curr);
                        break;
                    }
                } else {
                    node = node->next;
                }
            } else {
                ls_all(node);
                break;
            }
        }
    }

    // If the argument couldn't be found
    if (!found && *arg) {
        printf("ls: cannot access '%s': No such file or directory\n", arg);
    }
}

void pwd(TreeNode* treeNode) {
    if (!strcmp(treeNode->name, "root")) {
        // If it is only the root
        // the "/" won't be printed
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

TreeNode* cd(TreeNode* current_node, char* path, int tree_cmd) {
    // Special case if the current node is the root
    if (!strcmp(current_node->name, "root")) {
        if (!strncmp(path, PARENT_DIR, 2)) {
            if (!tree_cmd)
                printf("cd: no such file or directory: %s\n", path);
            else
                return NULL;
            return current_node;
        }
    }
    TreeNode *tree_node = current_node;
    List *tree_list = (List *) current_node->content;
    char *new_path = path;

    // If the content of the folder is empty
    if (!tree_list) {
        // Checking if the path is PARENT_DIR
        if (!strncmp(path, PARENT_DIR, 2)) {
            // path + 3 because besides ..
            // there will also be the '/' char
            new_path = path + 3;
            if (!strcmp(new_path, NO_ARG))
                tree_node = current_node->parent;
            else
                tree_node = cd(current_node->parent, new_path, 0);
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
        TreeNode *curr_node = node->info;
        // Infinite loop in order to check each node
        // of the list
        while (1) {
            // If the node is a folder
            if (node->info->type) {
                char *token;
                token = strtok(path, "/");
                while (token) {
                    if (!strcmp(token, PARENT_DIR)) {
                        tree_node = current_node->parent;
                    } else {
                        tree_node = find_folder(curr_node->parent, token);
                        if (!tree_node) {
                            if (!tree_cmd)
                                printf("cd: no such file or directory: %s\n",
                                        path);
                            else
                                return NULL;

                            tree_node = current_node;
                            break;
                        } else if (!tree_node->type) {
                            tree_node = current_node;
                        }
                        List *aux = (List *)node->info->content;
                        if (aux && aux->head) {
                            if (aux->head->info->type)
                                curr_node = aux->head->info;
                        }
                    }
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
                            tree_node = current_node->parent;
                        } else {
                            tree_node = cd(current_node->parent, new_path, 0);
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

void tree(TreeNode* current_node, char* arg) {
    int folder_nr = 0;
    int file_nr = 0;
    List *tree_list = current_node->content;
    ListNode *node = NULL;
    if (tree_list)
        node = tree_list->head;
    else
        return;
    if (!strcmp(arg, NO_ARG)) {
        // If there is no path
        tree_print(node, 0, &folder_nr, &file_nr);
        printf("\n%d directories, %d files\n", folder_nr, file_nr);
    } else {
        // Copying the path
        char *new_path = arg;
        TreeNode *tree_node = cd(current_node, new_path, 1);

        if (!tree_node) {
            printf("%s [error opening dir]\n", arg);
            printf("\n0 directories, 0 files\n");
        } else {
            if (!strcmp(tree_node->name, current_node->name)) {
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

void mkdir(TreeNode* current_node, char* folder_name) {
    TreeNode *new_dir = make_folder_node(current_node, folder_name);

    List *old_list = (List *) current_node->content;
    ListNode *node;
    if (old_list)
        node = old_list->head;

    if (!old_list) {
        List *old_list = create_new_list(current_node, new_dir);
        // If this is the first folder in the cuurent directory
        node = old_list->head;
        node->next = NULL;
        node->info = new_dir;
        current_node->content = old_list;
    } else {
        if (check_existance(current_node, folder_name)) {
            printf("mkdir: cannot create directory '%s': File exists",
                    folder_name);
        } else {
            while (node->next) {
                // In order to add it at the end of the list
                node = node->next;
            }
            // The loop will stop at the previous directory
            // So in order to add the new directory in the list of
            // directories, the last directory's next has to be changed
            node->next = (ListNode *)malloc(sizeof(ListNode));
            DIE(!node->next, "Malloc failed");

            node->next->info = new_dir;
            memcpy(node->next->info->name, new_dir->name,
                   strlen(new_dir->name) + 1);
            node->next->next = NULL;
            node->next->info->content = NULL;
        }
    }
    free(folder_name);
}

void touch(TreeNode* current_node, char* file_name, char* file_content) {
    // Creating new node
    TreeNode *new_file = make_file_node(current_node, file_name);
    // Allocating memory for the content
    new_file->content = calloc (TOKEN_MAX_LEN, sizeof (char));
    DIE(!new_file->content, "Calloc failed");

    // Copying the content
    memcpy(new_file->content, file_content, strlen(file_content) + 1);

    // Old list will be the content of the parent
    List *old_list = (List *) current_node->content;
    ListNode *node = NULL;

    // If the parent has other files
    if (old_list)
        node = old_list->head;

    // If the parent's content is empty
    if (!old_list) {
        List *old_list = create_new_list(current_node, new_file);
        memcpy((char *)old_list->head->info->content, file_content,
                strlen(file_content) + 1);

        // If this is the first folder in the cuurent directory
        node = old_list->head;
        node->next = NULL;
        node->info = new_file;
        current_node->content = old_list;
    } else {
        if (!check_existance(current_node, file_name)) {
            while (node->next)
                // In order to add it at the end of the list
                node = node->next;
            // The loop will stop at the previous file
            // So in order to add the new file in the list of
            // files, the last file's next has to be changed
            node->next = (ListNode *)malloc(sizeof(ListNode));
            DIE(!node->next, "Malloc failed");
            node->next->info = new_file;
            memcpy(node->next->info->name, new_file->name,
                   strlen(new_file->name) + 1);
            memcpy(node->next->info->content, new_file->content,
                   strlen(new_file->content) + 1);
            node->next->next = NULL;
        }
    }
    free(file_name);
    free(file_content);
}

void rmrec(TreeNode* current_node, char* resource_name) {
    List *tree_list = (List *) current_node->content;
    ListNode *node = tree_list->head;
    int is_dir = 0;
    TreeNode *file = node->info;
    ListNode *prev = NULL;
    ListNode *target = node;
    int found = 0;

    if (node) {
        while (node) {
            if (!strcmp(node->info->name, resource_name)) {
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
                resource_name);
        return;
    }
    // First linking the previous node to the next one
    if (!prev) {
        if (target->next) {
            tree_list->head = target->next;
        } else {
            free_list(tree_list);
            tree_list = NULL;
            current_node->content = NULL;
        }
    } else {
        prev->next = target->next;
    }

    if (is_dir) {
        // Then deleting the node
        free_folder(file);
    } else {
        // Then deleting the node
        free_file(file);
    }

    file = NULL;
    if (node) {
        node = NULL;
    }
    if (tree_list)
        if (!tree_list->head)
            tree_list = NULL;
    target = NULL;
}


void rm(TreeNode* current_node, char* file_name) {
    List *tree_list = (List *) current_node->content;
    ListNode *node = (ListNode *) tree_list->head;
    int found = 0;
    TreeNode *file;

    if (node) {
        file = node->info;
        while (node) {
            if (!strcmp(node->info->name, file_name)) {
                // Searching for name in the list
                if (!node->info->type) {
                    // Checking if it is a file, not a directory
                    found = 1;
                    file = node->info;
                } else {
                    printf("rm: cannot remove '%s': Is a directory", file_name);
                    return;
                }
                break;
            }
            node = node->next;
        }
    }

    if (found) {
        rmrec(current_node, file_name);
        if (tree_list)
            tree_list = NULL;
        if (node)
            node = NULL;
        if (file)
            file = NULL;
    } else {
        printf("rm: failed to remove '%s': No such file or directory",
                file_name);
        return;
    }
}


void rmdir(TreeNode* current_node, char* folder_name) {
    List *tree_list = (List *) current_node->content;
    if (!tree_list) {
        printf("rmdir: failed to remove '%s': No such file or directory",
                folder_name);
        return;
    }
    ListNode *node = (ListNode *) tree_list->head;
    int found = 0;
    TreeNode *folder = node->info;

    if (node) {
        while (node) {
            if (!strcmp(node->info->name, folder_name)) {
                // Searching for name in the list
                if (node->info->type) {
                    // Checking if it is a folder, not a file
                    found = 1;
                    folder = node->info;
                } else {
                    printf("rmdir: failed to remove '%s': Not a directory\n",
                            folder_name);
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
            rmrec(current_node, folder_name);
            if (tree_list)
                tree_list = NULL;
            if (node)
                node = NULL;
            if (folder)
                folder = NULL;
        } else {
            printf("rmdir: failed to remove '%s': Directory not empty",
                    folder_name);
            return;
        }
    } else {
        printf("rmdir: failed to remove '%s': No such file or directory",
                folder_name);
        return;
    }
}

void cp(TreeNode* current_node, char* source, char* destination) {
    // TODO(stuff) : stuff
}

void mv(TreeNode* current_node, char* source, char* destination) {
    // TODO(stuff) : stuff
}
