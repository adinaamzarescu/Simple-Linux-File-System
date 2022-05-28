#include "tree.h"
#include "helpers.h"

void free_list(List *list) {
    if (!list)
        return;
    if (list->head) {
        free(list->head);
        list->head = NULL;
    }
    if (list) {
        free(list);
        list = NULL;
    }
}

void free_file(TreeNode *file) {
    if (!file)
        return;
    List *old_list = NULL;
    ListNode *node = NULL;
    ListNode *prev = NULL;

    if (file->parent)
        old_list = (List *)file->parent->content;

    // It this is the first file in the parent's
    // content then the head of the list should
    // be freed
    if (old_list) {
        if (old_list->head) {
            node = old_list->head;
            if (!strcmp(node->info->name, file->name)) {
                if (!node->next) {
                    free_list(old_list);
                    old_list = NULL;
                } else {
                    prev = old_list->head;
                    old_list->head = node->next;
                    free(prev);
                }
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

void free_folder(TreeNode *folder) {
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
                        free_folder(prev->info);
                    } else {
                        free_file(prev->info);
                    }
                }
                // This loop will end at the previous node
                // For the last node
                if (node->info->type) {
                    free_folder(node->info);
                } else {
                    free_file(node->info);
                }
                prev = NULL;
            } else {
                // If this is the only file in the list
                if (node->info->type) {
                    free_folder(node->info);
                } else {
                    free_file(node->info);
                }
            }
            if (node) {
                node = NULL;
            }
        }
        // The new list will become null
        new_list = NULL;
    }

    // Now the folder is empty
    // But just to be safe
    if (folder->content)
        folder->content = NULL;

    // Testing if the folder was the only
    // one in the content of the parent
    if (old_list) {
        if (old_list->head) {
            node = old_list->head;
            if (!strcmp(node->info->name, folder->name)) {
                if (!node->next) {
                    free_list(old_list);
                    old_list = NULL;
                } else {
                    prev = old_list->head;
                    old_list->head = node->next;
                    free(prev);
                }
            }
        }
    }
    // Now clearing the folder
    free(folder->name);
    folder->name = NULL;

    free(folder);
    folder = NULL;
}
