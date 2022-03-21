#ifndef SPLAY_HEAD
#define SPLAY_HEAD

#include"BST_head_file.cpp"

template<typename T>
class Splay : public BST<T>{
    public:   //伸展树的搜索会导致树结构变化，所以要重写 BST 的搜索算法
        Splay(): BST<T>() { }
        Splay( BinNode<T> * & root ): BST<T>( root ) { }
        ~Splay(){ }

        BinNode<T>* & search( const T & e );   //搜索重写
        BinNode<T>* & insert( const T & e );   //插入重写
        bool remove( const T & e );   //删除重写
    protected:
        void splay( BinNode<T>* v );   //伸展至根；无需返回值，利用根节点即可
};

template<typename T>
void Splay<T>::splay( BinNode<T>* v ){
    if( !v ){
        return ;
    }
    BinNode<T>* p, * g, * gg = nullptr;   //父亲，祖父, 曾祖父
    while( ( p = v->parent ) && ( g = p->parent ) ){   //短路原则，p 为空时仍然可行
        gg = g->parent;   //曾祖父，每轮之后，都将以曾祖父为父;即每轮之后，g 的位置将成为新的 v
        if( p == g->lChild  ){   //注意有两种情况不能调用 3+4 重构，而 3+4 内更新高度的操作会与下方更新高度的操作重复，
                                    //所以统一不调用 3+4 重构
            if( v == p->lChild ){
                p->parent = v; g->parent = p;
                p->lChild = v->rChild; g->lChild = p->rChild;
                v->rChild = p; p->rChild = g;
            }else{
                p->parent = v; g->parent = v;
                p->rChild = v->lChild; g->lChild = v->rChild;
                v->lChild = p; v->rChild = g;
            }
        }else{   //上面两种情况与下面两种情况分别对称，直接将代码的左右互换即可
            if( v == p->rChild ){
                p->parent = v; g->parent = p;
                p->rChild = v->lChild; g->rChild = p->lChild;
                v->lChild = p; p->lChild = g;
            }else{
                p->parent = v; g->parent = v;
                p->lChild = v->rChild; g->rChild = v->lChild;
                v->rChild = p; v->lChild = g;
            }
        }
        if( !gg ){   //因为每轮双层旋转后，都将以曾祖父为新的父亲，所以若无曾祖父 gg，则 v 现在即为根
            v->parent = nullptr;
        }else{   //若有曾祖父，向上连接
            g == gg->lChild ? gg->lChild = v : gg->rChild = v;
            v->parent = gg;
        }
        BinTree<T>::updateHeight( g );   //不断更新高度
        BinTree<T>::updateHeight( p );   //注意顺序
        BinTree<T>::updateHeight( v );
    }   //双层伸展结束时， 必有 g == nullptr， p 可能为空
    if( (p = v->parent) ){   //若 v 还有父节点，即特殊情况，做一次单旋，使 v 成为树根。多一个括号消除警告
        if( v == p->lChild ){   //左旋 zag
            p->lChild = v->rChild;
            v->rChild = p;
        }else{   //右旋 zig
            p->rChild = v->lChild;
            v->lChild = p;
        }
        p->parent = v;
        v->parent = nullptr;   //注意：到此处单旋才结束。单旋时这里肯定是根节点，所以要做这样的操作。
        BinTree<T>::updateHeight( p );   //更新高度
        BinTree<T>::updateHeight( v );   //注意顺序
    }
    BinTree<T>::_root = v;
    return ;
}

template<typename T>
BinNode<T>* & Splay<T>::search( const T & e ){
    BinNode<T>* & x = BST<T>::search( e );   // 调用 BST 内的函数
    if( x ){
        splay( x );
    }
    return x;   //此处必须返回 x，因为有可能该节点不存在，要返回一个空指针的引用
}

template<typename T>
BinNode<T>* & Splay<T>::insert( const T & e ){
    BinNode<T>* & x = BST<T>::search( e );   //这里是 BST 的搜索函数
    if( !x ){
        x = new BinNode<T>( e, BST<T>::_hot );   //上方 serach 更新了 _hot 辅助变量
        BinTree<T>::_size++;
        splay( x );   //这是插入后伸展
    }
    return BinTree<T>::_root;
}

template<typename T>
bool Splay<T>::remove( const T & e ){
    BinNode<T>* x = BST<T>::search( e );   //不要调用 Splay 的 search 函数，因为那里面进行了伸展；
        //因为是返回引用，所以其中的伸展操作极有可能改变被引用指针的值，最后导致此处 x 为空，这样就会找到了但没有删除
    if( !x ){
        return false;
    }
    splay( (*x).succ() );    //得到后继，并伸展
    //BST<T>::remove( e );   如果调用 BST 的 remove 算法，将会再次进行一次不必要的 BST 中的搜索
    BST<T>::removeAt( x, BST<T>::_hot );   //上方搜索过，更新了 _hot，所以可以传入 _hot。
    return true;
}

#endif