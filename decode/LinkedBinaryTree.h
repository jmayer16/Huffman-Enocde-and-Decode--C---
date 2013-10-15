#ifndef LINKED_BINARY_TREE_H
#define LINKED_BINARY_TREE_H
#include <stdexcept>

// A LinkedBinaryTree implementation inspired (but not identical to)
// that of Chapter 7 of our text.

template <typename Object>
class LinkedBinaryTree {

private: 
    // Private node struct represents single node of the LinkedBinaryTree
    struct Node {                             // node in the LinkedBinaryTree
        Object elem;                          // element value
        Node* parent;                         // parent
        Node* left;                           // left child
        Node* right;                          // right child

        int aux;                              // auxilary info (for future use)

	// constructor
        Node(const Object& e = Object(), Node* p = NULL, Node* l = NULL,
	     Node* r = NULL, int aux = 0)
	: elem(e), parent(p), left(l), right(r), aux(aux) { }
    };

public:
    // Public Position represents the position of a node in LinkedBinaryTree
    class Position {
    private:
        Node* node;                       // pointer to current node
        const LinkedBinaryTree* tree;     // pointer to tree Position belongs to
        friend class LinkedBinaryTree;    // allow access to private member

    public:
        // constructor
        Position(Node* n = NULL, const LinkedBinaryTree* t = NULL)
	    : node(n), tree(t) { }

        // return reference to data
        Object& operator*() const {
            return node->elem;
        }

        // return pointer to data
        Object* operator->() const {
            return &(node->elem);
        }

        // Return true if the positions are equivalent
        bool operator==(const Position& other) {
            return (node == other.node && tree == other.tree);
        }

        // Return true if the positions are not equivalent
        bool operator!=(const Position& other) {
            return !(*this == other);
        }

        // Return Position of left child
        Position left() const {
            return Position(node->left, tree);
        }

        // Return Position of right child
        Position right() const {
            return Position(node->right, tree);
        }

        // Return Position of parent child
        Position parent() const {
            return Position(node->parent, tree);
        }

        // Return True if position has a left child
        bool hasLeftChild() const {
            return (node->left != NULL);
        }

        // Return True if position has a right child
        bool hasRightChild() const {
            return (node->right != NULL);
        }

        // Return True if position is the root of the tree
        bool isRoot() const {
            return (node == tree->rt);
        }

        // Return True if position has a right child
        bool isExternal() const {
            return (node->left == NULL && node->right == NULL);
        }

        // Return True if position is invalid
        bool isNull() const {
            return (node == NULL);
        }

        int getAuxillary() const {
            return node->aux;
        }

        void setAuxillary(int value) {
            node->aux = value;
        }

    };
    /*********** end of class Position ***************************************/

private:
    Node*   rt;       // root of the tree
    int     n;        // number of nodes

    // Utility for error checking
    Node* validate(const Position& p, bool forceExternal=false) {
	if (p.isNull())
	    throw std::runtime_error("Null Position");
	if (p.tree != this)
	    throw std::runtime_error("Position does not belong to this tree");
	if (forceExternal && !p.isExternal())
	    throw std::runtime_error("Position is not external");
	return p.node;
    }


    // Utilities used for copy constructor, assignment operator, and destructor
    Node* cloneRecurse(const Node* nd) {
        if (nd == NULL)
            return NULL;
        else {
            Node* temp = new Node(nd->elem, NULL, cloneRecurse(nd->left),
                                  cloneRecurse(nd->right), nd->aux);
            if (temp->left) temp->left->parent = temp;
            if (temp->right) temp->right->parent = temp;
            return temp;
        }
    }

    void clearRecurse(const Node* nd) {
        if (nd != NULL) {
            clearRecurse(nd->left);
            clearRecurse(nd->right);
            delete nd;
        }
    }

public:
    // Default constructor creates empty tree
    LinkedBinaryTree() : rt(NULL), n(0) { }

    // Copy Constructor
    LinkedBinaryTree(const LinkedBinaryTree& other)
	: rt(cloneRecurse(other.rt)), n(other.n) { }

    // Assignment Operator
    LinkedBinaryTree& operator=(const LinkedBinaryTree& other) {
        if (this != &other) {
            clearRecurse(rt);
            rt = cloneRecurse(other.rt);
            n = other.n;
        }
        return *this;
    }

    // Destructor
    ~LinkedBinaryTree() {
        clearRecurse(rt);
    }

    /**************************** accessor methods **************************/

    // return number of nodes
    int size() const {
        return n;
    }

    // return true if empty tree
    bool empty() const {
        return n == 0;
    }

    // return position of root
    Position root() const {
        return Position(rt, this);
    }

    /****************************** update methods ****************************/

    void addRoot(const Object& value=Object()) {
        rt = new Node(value);
        n = 1;
    }

    /* Converts external position into an internal node with two newly
     * created external children (each of which have default element)
     */
    void expandExternal(const Position& p) {
	Node* ext = validate(p, true);             // verify external node
        ext->left = new Node(Object(), ext);
        ext->right = new Node(Object(), ext);
        n += 2;
    }

    /* Takes an external position p of tree, deletes p and the parent of
     * p from the tree, promoting the sibling of p into the parent's
     * place (see Figure 7.15)
     */
    Position removeAboveExternal(const Position& p) {
	Node* ext = validate(p, true);             // verify external node
        Node* parent = ext->parent;
        Node* sib = (ext == parent->left ? parent->right : parent->left);

        if (parent == rt) {              // child of tree's root?
            rt = sib;
            sib->parent = NULL;
        } else {
            Node* grand = parent->parent;     // external's grandparent
            if (parent == grand->left)
                grand->left = sib;
            else
                grand->right = sib;
            sib->parent = grand;
        }
        delete ext;
        delete parent;
        n -= 2;
        return Position(sib, this);
    }

    /* Replaces the external position p with a subtree which mirrors the
     * contents of a second tree T2.
     *
     *  Note well: the external node as well as the second tree itself are
     *  destroyed as a side effect.
     */
    void replaceExternalWithSubtree(const Position& p, LinkedBinaryTree& T2) {
	Node* ext = validate(p, true);             // verify external node
        n += (T2.n - 1);
        if (ext == rt) {
            rt = T2.rt;
        } else {
            Node* parent = ext->parent;
            T2.rt->parent = parent;
            if (ext == parent->left)
                parent->left = T2.rt;
            else
                parent->right = T2.rt;
        }

        // deallocate original external node
        delete ext;

        // T2 becomes empty
        T2.n = 0;
        T2.rt = NULL;
    }
};

#endif
