#include "helpers.h"
#include "tree.h"

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

// Function that prints the path
void print_rec(TreeNode *treeNode) {
    if (treeNode->parent) {
        print_rec(treeNode->parent);
        printf("%s/", treeNode->name);
    } else {
        return;
    }
}

TreeNode *findFolder(TreeNode* currentNode, char* token) {
    List *tree_list = (List *) currentNode->content;
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
    // ListNode *aux = target;
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
