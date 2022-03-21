#include<iostream>
#include"headFile\BinTree_head_file.cpp"

template<typename T>
void print(BinTree<T> & tree );   //先序输出
template<typename T>
void print_help( BinNode<T> * & root );
template<typename T>
void succ_experiment(BinNode<T>* node_arr[] );   //测试 BinNode 类的 succ 函数 

// a ~ g 对应 1 ~ 7
int main(void){
    BinNode<int>* node_arr[8];   //记录每个节点的位置
    node_arr[0] = nullptr;

    node_arr[1] = new BinNode<int>( 1, nullptr );   //根节点
    BinTree<int> tree( node_arr[1] );   //建立树
    node_arr[2] = tree.insertAsLC( node_arr[1], 2 );
    node_arr[3] = tree.insertAsRC( node_arr[1], 3 );
    node_arr[4] = tree.insertAsLC( node_arr[2], 4 );
    node_arr[5] = tree.insertAsLC( node_arr[3], 5 );
    node_arr[6] = tree.insertAsLC( node_arr[5], 6 );
    node_arr[7] = tree.insertAsRC( node_arr[5], 7 );

    succ_experiment<int>( node_arr );

    print<int>( tree );

    system("pause");
    return 0;
}

template<typename T>
void succ_experiment(BinNode<T> * node_arr[] ){   //测试 BinNode 类的 succ 函数 
    for( int i = 1; i <= 7; i++ ){   //注意：当前节点为中序最后一个节点时，返回的后继为空
        BinNode<T> * t = node_arr[i]->succ();   //要把树结构变换多种才能测出 succ 是否正确
        if( t == nullptr ){
            std::cout<< i <<" 的中序后继为： 当前节点后继为空！"<<std::endl;
        }else{
            std::cout<< i <<" 的中序后继为： "<< t->data <<std::endl;
        }
    }
    return ;
}

template<typename T>
void print_help( BinNode<T> * & root ){
    if( root == nullptr ){
        return ;
    }
    std::cout<<"data = "<< root->data <<", height = "<< root->height <<std::endl;
    print_help<T>( root->lChild );
    print_help<T>( root->rChild );
}

template<typename T>
void print(BinTree<T> & tree ){
    BinNode<T> * root = tree.root();
    print_help<T>( root );
    std::cout<<"size = "<< tree.size() <<std::endl;
    return ;
}