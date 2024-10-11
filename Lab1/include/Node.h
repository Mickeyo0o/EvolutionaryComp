#ifndef NODE_H
#define NODE_H


class Node
{
    public:
        Node(int x, int y, int node_cost);
        virtual ~Node();
        void displayNode() const;
        int x;
        int y;
        int cost;

    protected:

    private:
};

#endif // NODE_H
