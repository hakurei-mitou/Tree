#include<iostream>
#include"headFile\BT_head_file.cpp"

void superNode( BTNode<int>* root );   //遍历超级节点

void print( BTNode<int>* root ){
    superNode( root );
    std::cout<<"\n\n";
}

int main(void){
    //std::cout<<"point 7"<<std::endl;
        BT<int> tree( 5, 7 );   // 5 阶 B 树，根节点数据为 7    
        //print( tree.get_root() );
    //std::cout<<"point 8"<<std::endl;
        tree.insert( 8 );
        //print( tree.get_root() );
    //std::cout<<"point 4"<<std::endl;
        tree.insert( 4 );
        //print( tree.get_root() );
    //std::cout<<"point 2"<<std::endl;
        tree.insert( 2 );
        //print( tree.get_root() );
    //std::cout<<"point 0"<<std::endl;
        tree.insert( 0 );
        //print( tree.get_root() );
    //std::cout<<"point 3"<<std::endl;
        tree.insert( 3 );
        //print( tree.get_root() );
    //std::cout<<"point 9"<<std::endl;
        tree.insert( 9 );
        print( tree.get_root() );
    /*   第 1 段测试
    std::cout<<"remove 2"<<std::endl;
        tree.remove( 2 );
        print( tree.get_root() );
    std::cout<<"remove 4"<<std::endl;
        tree.remove( 4 );   //删除 4 为删除根节点仅剩的一个关键码的特殊情况
        print( tree.get_root() );
//    std::cout<<"remove 4"<<std::endl;
        //if( ! tree.remove( 4 ) ){
    //    std::cout<< "remove the 4 fail\n\n";
    //}
    std::cout<<"remove 0"<<std::endl;
        tree.remove( 0 );   //删除 0，测试合并
        print( tree.get_root() );
    */
    //std::cout<<"insert 10"<<std::endl;
        tree.insert( 10 );
        //print( tree.get_root() );
    //std::cout<<"insert 11"<<std::endl;
        tree.insert( 11 );
        //print( tree.get_root() );
    //std::cout<<"remove 10"<<std::endl;
        tree.remove( 10 );
        print( tree.get_root() );

    std::cout<<"remove 0"<<std::endl;
        tree.remove( 0 );
        print( tree.get_root() );
    std::cout<<"remove 2"<<std::endl;
        tree.remove( 2 );
        print( tree.get_root() );

    /*
    for( int i = 0; i < 17; i++ ){
        std::cout<<"point "<< i <<std::endl;
        if( tree.insert( i ) ){
            print( tree.get_root() );
        }else{
            std::cout<< i <<"  插入失败！\n\n";
        }
    }
    */

    return 0;
}

void superNode( BTNode<int>* root ){   //遍历超级节点，以便对应草图分析
    if( ! root ){
        return ;
    }
    bool flag = false;   //标记当前超级节点是否被输出过
    for( auto i = root->child.begin(); i < root->child.end(); i++ ){
        superNode( *i );
        if( !flag ){
            flag = true;
            for( auto i = root->key.begin(); i < root->key.end(); i++ ){
                std::cout<< *i <<" ";
            }
            std::cout<<"## ";
        }
    }
}