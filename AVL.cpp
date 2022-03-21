#include<iostream>
#include"headFile\AVL_head_file.cpp"
//#include"headFile\BST_head_file.cpp"   上方头文件已经包含了它，但也可以继续包含
//#include"headFile\BinTree_head_file.cpp"   上方头文件已经包含了它，但也可以继续包含



template<typename T>
void inorder_traversal(BinNode<T>* root );   //以中序遍历测试

//通过中序遍历序列和高度，可以确定其二叉树形状,继而调试

// a ~ g 对应 1 ~ 7
int main(void){
    AVL<int> tree(new BinNode<int>( 3, nullptr ));   //建立根节点
    std::cout<<"初始 root 3 \n\n";

    tree.insert( 1 );   //注意：这里用 AVL 的 insert 才能保证构造的是一颗 AVL
    std::cout<<"\ninsert 1 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性

    tree.insert( 2 );   //测试插入函数
    std::cout<<"\ninsert 2 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性

    tree.insert( 4 );
    std::cout<<"\ninsert 4 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性
    
    tree.insert( 5 );
    std::cout<<"\ninsert 5 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性
    
    tree.insert( 6 );
    std::cout<<"\ninsert 6 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性

    tree.insert( 7 );
    std::cout<<"\ninsert 7 \n";
    inorder_traversal<int>( tree.get_root() );   //注意检查插入算法是否保证了平衡性

    tree.remove( 3 );   //测试删除函数
    std::cout<<"\nremove 3 \n";
    inorder_traversal<int>( tree.get_root() );

    tree.remove( 6 );   //测试删除函数
    std::cout<<"\nremove 6 \n";
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