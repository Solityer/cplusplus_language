namespace AQueue
{
    struct Node
    {
        struct Node* next;
        int val;
    };

    struct Queue
    {
        struct Node* head;
        struct Node* tail;
    };
}