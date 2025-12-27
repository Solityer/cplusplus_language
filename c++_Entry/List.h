#pragma once

namespace BList
{
    struct Node
    {
        struct Node* next;
        struct Node* prev;
        int val;
    };
}
