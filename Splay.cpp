#include<iostream>
#include"headFile\Splay_head_file.cpp"

template<typename T>
void inorder_traversal(BinNode<T>* root );   //以中序遍历测试

//通过中序遍历序列和高度，可以确定其二叉树形状,继而调试

// a ~ g 对应 1 ~ 7
int main(void){
    BinNode<int>* node_arr[8];   //记录每个节点的位置
    node_arr[0] = nullptr;

    node_arr[1] = new BinNode<int>( 3, nullptr );   //根节点
    Splay<int> tree( node_arr[1] );   //建立 Splay
    std::cout<<"初始 root 3 \n\n";

    node_arr[2] = tree.insert( 1 );   //注意：这里用 AVL 的 insert 才能保证构造的是一颗 Splay
    std::cout<<"\ninsert 1 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性

    node_arr[3] = tree.insert( 2 );   //测试插入函数
    std::cout<<"\ninsert 2 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性

    node_arr[4] = tree.insert( 4 );
    std::cout<<"\ninsert 4 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性
    
    node_arr[5] = tree.insert( 5 );
    std::cout<<"\ninsert 5 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性
    
    node_arr[6] = tree.insert( 6 );
    std::cout<<"\ninsert 6 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性

    node_arr[7] = tree.insert( 7 );
    std::cout<<"\ninsert 7 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性

    tree.remove( 3 );   //测试删除函数
    std::cout<<"\nremove 3 \n";
    inorder_traversal<int>( tree.get_root() );

    tree.remove( 6 );   //测试删除函数
    std::cout<<"\nremove 6 \n";
    inorder_traversal<int>( tree.get_root() );

    tree.search( 4 );   //测试搜索函数
    std::cout<<"\nsearch 4 \n";
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
    std::cout<< root->data <<" height : "<< root->height <<std::endl;
    inorder_traversal( root->rChild );
}