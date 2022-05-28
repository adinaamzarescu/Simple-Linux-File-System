#include "helpers.h"
#include "tree.h"

void ls_all(ListNode* node) {
    if (node && node->next) {
        ls_all(node->next);
        printf("%s\n", node->info->name);
    } else if (!node) {
        return;
    } else {
        printf("%s\n", node->info->name);
    }
}

void ls_folder(TreeNode* current_node) {
    List *list = current_node->content;
    ListNode *node = list->head;
    ls_all(node);
}

void ls_file(TreeNode* current_node) {
    char *cnt = (char *) current_node->content;
    printf("%s: %s\n", current_node->name, cnt);
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

TreeNode *find_folder(TreeNode* current_node, char* token) {
    List *tree_list = (List *) current_node->content;
    ListNode *node = NULL;
    if (tree_list)
        node = tree_list->head;
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

void tree_print(ListNode *target, int level, int *folder_nr, int *file_nr) {
    int number1 = *folder_nr;
    int number2 = *file_nr;
    // First the program will show the directories
    ListNode *node = target;
    ListNode *prev = node;
    while (node) {
        // In order to add the spaces
        int level_now = level;
        while (level_now) {
            printf("    ");
            --level_now;
        }
        prev = node;

        while (node->next) {
            prev = node;
            node = node->next;
            if (node) {
                printf("%s\n", node->info->name);
                // For each directory, it will do the same for
                // the sub-directories
                if (node->info->type) {
                    number1++;
                    List *list = node->info->content;
                    ListNode *new_node = list->head;
                    tree_print(new_node, level + 1, &number1, &number2);
                } else {
                    number2++;
                }
            } else {
                printf("%s\n", prev->info->name);
                // For each directory, it will do the same for
                // the sub-directories
                if (prev->info->type) {
                    number1++;
                    List *list = prev->info->content;
                    ListNode *new_node = list->head;
                    tree_print(new_node, level + 1, &number1, &number2);
                } else {
                    number2++;
                }
            }
        }

        if (prev->next) {
            level_now = level;
            while (level_now) {
                printf("    ");
                --level_now;
            }
        }

        printf("%s\n", prev->info->name);
        if (prev->info->type) {
            number1++;
            List *list = prev->info->content;
            ListNode *new_node = NULL;
            if (list) {
                new_node = list->head;
                tree_print(new_node, level + 1, &number1, &number2);
            }
        } else {
            number2++;
        }
        node = node->next;
    }

    *folder_nr = number1;
    *file_nr = number2;
}

TreeNode *make_file_node(TreeNode* current_node, char* fileName) {
    TreeNode *new_node = malloc(sizeof(*new_node));
    DIE(!new_node, "Malloc failed");

    new_node->name = malloc(TOKEN_MAX_LEN);
    DIE(!new_node->name, "Malloc failed");

    memcpy(new_node->name, fileName, strlen(fileName) + 1);

    new_node->parent = current_node;
    new_node->type = 0;
    new_node->content = NULL;

    return new_node;
}

TreeNode *make_folder_node(TreeNode* current_node, char* folderName) {
    TreeNode *new_node = malloc(sizeof(*new_node));
    DIE(!new_node, "Malloc failed");

    new_node->name = malloc(TOKEN_MAX_LEN);
    DIE(!new_node->name, "Malloc failed");

    memcpy(new_node->name, folderName, strlen(folderName) + 1);

    new_node->parent = current_node;
    new_node->type = 1;
    new_node->content = NULL;

    return new_node;
}

int check_existance(TreeNode* current_node, char* folderName) {
    // This function will return 1 if there
    // is a folder with the same name
    // and 0 if not
    List *new_list = (List *) current_node->content;
    if (new_list->head) {
        ListNode *node = new_list->head;
        TreeNode *curr_node = node->info;
        if (!strcmp(curr_node->name, folderName))
            return 1;
        while (node->next) {
            node = node->next;
            if (!strcmp(curr_node->name, folderName))
                return 1;
            curr_node = node->info;
        }
        if (!strcmp(curr_node->name, folderName))
                return 1;
    }
    return 0;
}

List * create_new_list(TreeNode* current_node, TreeNode *new_file) {
    List* new_list = malloc(sizeof(*new_list));
    DIE(!new_list, "Malloc failed");

    new_list->head = (ListNode*)malloc(sizeof(ListNode));
    DIE(!new_list->head, "Malloc failed");

    new_list->head->next = NULL;
    new_list->head->info = new_file;
    new_list->head->info->parent = current_node;

    return new_list;
}
