#include<cstdio>
#include<iostream>
#include"headFile\RBT_head_file.cpp"

template<typename T>
void inorder_traversal(BinNode<T>* root );   //以中序遍历测试

int main(void) {
    RBT<int> tree;

    std::cout<<"\ninsert 72:\n";
    tree.insert(72);
    inorder_traversal(tree.get_root());

    std::cout<<"\ninsert 83:\n";
    tree.insert(83);
    inorder_traversal(tree.get_root());

    std::cout<<"\ninsert 87: RR-1\n";
    tree.insert(87);   //RR-1
    inorder_traversal(tree.get_root());

    std::cout<<"\ninsert 92: RR-2\n";
    tree.insert(92);   //RR-2
    inorder_traversal(tree.get_root());

    std::cout<<"\ninsert 86:\n";
    tree.insert(86);
    inorder_traversal(tree.get_root());

    std::cout<<"\nremove 87: succ is null\n";
    tree.remove(87);   //后继为空，特殊处理
    inorder_traversal(tree.get_root());

    std::cout<<"\nremove 92: RB\n";
    tree.remove(92);   //RB
    inorder_traversal(tree.get_root());

    std::cout<<"\ninsert 84:\n";
    tree.insert(84);
    inorder_traversal(tree.get_root());

    std::cout<<"\ninsert 90:\n";
    tree.insert(90);
    inorder_traversal(tree.get_root());

    std::cout<<"\ninsert 85: RR-2\n";
    tree.insert(85);   //RR-2
    inorder_traversal(tree.get_root());

    std::cout<<"\nremove 72: it is a leaf\n";
    tree.remove(72);
    inorder_traversal(tree.get_root());

    //BB 请读者自证（实在有点难构造啊……有什么错误欢迎指正，要注意高度的更新函数是否缺失了）
    return 0;
}

template<typename T>
void inorder_traversal(BinNode<T>* root ){
    if( root == nullptr ){
        return ;
    }
    inorder_traversal( root->lChild );
    printf("%3d %3d  %s\n", root->data, root->height, (root->color == Red ? "Red" : "Black"));
    inorder_traversal( root->rChild );
}