#include<iostream>
#include"headFile\BST_head_file.cpp"
//#include"BinTree_head_file.cpp"   上方头文件已经包含了它，但也可以继续包含


template<typename T>
void inorder_traversal(BinNode<T>* root );   //以中序遍历测试

// a ~ g 对应 1 ~ 7
int main(void){
    BinNode<int>* node_arr[8];   //记录每个节点的位置
    node_arr[0] = nullptr;

    node_arr[1] = new BinNode<int>( 3, nullptr );   //根节点
    BST<int> tree( node_arr[1] );   //建立 BST
    node_arr[2] = tree.insert( 1 );   //注意：这里用 BST 的 insert 才能保证构造的是一颗 BST
    node_arr[3] = tree.insert( 2 );
    node_arr[4] = tree.insert( 4 );
    node_arr[5] = tree.insert( 6 );
    node_arr[6] = tree.insert( 5 );
    node_arr[7] = tree.insert( 7 );

    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了顺序性

    tree.remove( 3 );   //这个函数是借用其它函数，可同时测试多个函数的正确性        
    std::cout<<std::endl;
    inorder_traversal<int>( tree.get_root() );

    tree.remove( 4 );
    std::cout<<std::endl;
    inorder_traversal<int>( tree.get_root() );

    system("pause");
    return 0;
}

template<typename T>
void inorder_traversal(BinNode<T>* root ){
    if( root == nullptr ){
        return ;
    }
    inorder_traversal( root->lChild );
    std::cout<< root->data <<"  height: "<< root->height <<std::endl;
    inorder_traversal( root->rChild );
}