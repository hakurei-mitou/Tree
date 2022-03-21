#ifndef AVL_HEAD
#define AVL_HEAD

#include"BST_head_file.cpp"

//注意以下几个宏定义, p 是指向某个节点的指针， x 是某个节点
#define stature(p) ( (p) ? (p)->height : -1 )   //统一高度语义
#define Balanced(p) ( stature( (p)->lChild ) == stature( (p)->rChild ) )   //理想平衡
#define BalFac(p) ( stature( (p)->lChild ) - stature( (p)->rChild ) )   //平衡因子
#define AvlBalanced(p) ( ( -2 < BalFac(p) ) && ( BalFac(p) < 2 ) )   //适度平衡条件

template<typename T>
class AVL : public BST<T> {
    public:
        AVL(): BST<T>(){ }
        AVL( BinNode<T>* root ): BST<T>( root ){ }
        ~AVL(){ }

        BinNode<T>* & insert( const T & );   //插入重写       //注意是否正确覆盖
        bool remove( const T & );   //删除重写
        //注意：两个重要的辅助算法：3+4重构和旋转统一调整在 BST 类中
    protected:
        BinNode<T>* tallerChild( BinNode<T>* );   //返回传入节点的高度最高的那个孩子
};

template<typename T>
BinNode<T>* & AVL<T>::insert( const T & e ){   //注意：此函数中会出现短暂的变量语义不正确
    BinNode<T>* & x = BST<T>::search( e );
    if( x ){   //已存在目标，直接退出
        return x;
    }
    x = new BinNode<T>( e, BST<T>::_hot );   //新建节点
    BinTree<T>::_size++;   //更新规模
    int count = 0;
    for( BinNode<T>* g = x->parent, * v = x; g ; g = g->parent ){   //一开始 g 的语义不正确，但一次判断失衡后，恢复正确
            // v 用来记录孙子，特殊情况时 v 的语义不正确，但特殊情况后语义保持正确；可以知道，语义不正确时，仍可以正确处理特殊情况
            // 特殊情况：从空树开始，插入第一第二个节点时不可能失衡，此时 v 的语义不正确，但不会用到 v
        if( !AvlBalanced( g ) ){   //判断失衡
            g = BST<T>::rotateAt( v );   //旋转统一处理
            break;   //只会有一个祖先失衡，复衡后即可停止向上追溯
        }else{
            BinTree<T>::updateHeight( g );   // 更新高度，平衡性虽不变，但高度可能改变,这个函数只更新单个节点的高度
            if( ++count > 1 ){   // v 记录孙子，注意此处 v 的更新
                v = v->parent;
            }
        }
    }
    return x;   //返回插入位置
}

template<typename T>
BinNode<T>* AVL<T>::tallerChild( BinNode<T>* v ){   //返回传入节点的高度最高的那个孩子
    BinNode<T>* left = v->lChild, * right = v->rChild;
    int a = -1, b = -1;   //注意初始化值
    a = stature( left );
    b = stature( right );
    return a < b ? right : left;   //其返回值不可能为空
}

template<typename T>
bool AVL<T>::remove( const T & e ){
    BinNode<T>* & x = BST<T>::search( e );   //得到位置
    if( !x ){   //若为空则直接退出，删除失败
        return false;
    }
    BST<T>::removeAt( x, BST<T>::_hot );   //删除节点
    BinTree<T>::_size--;
    int count = 0;
    for( BinNode<T>* g = BST<T>::_hot; g; g = g->parent ){   //必须向上不断追溯
        if( !AvlBalanced( g ) ){
            g = BST<T>::rotateAt( tallerChild( tallerChild( g ) ) );   //注意，删除与插入不同，无法通过记录 v 来避免使用 tallerChild 函数
        }
        BinTree<T>::updateHeight( g );   //旋转后高度未必复原，更高祖先仍可能失衡,注意：这个函数只更新单个节点的高度
    }
    return true;
}

#endif