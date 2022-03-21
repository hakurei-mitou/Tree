#ifndef BST_HEAD
#define BST_HEAD

#include"BinTree_head_file.cpp"   //注意此处包含的头文件

#include<iostream>
#include<algorithm>

template<typename K, typename V>
class Entry{                        // 词条类，可作为模板数据类型参数使用。为简化，本例未使用
    K key;   //关键码
    V value;   //数值,数据
    Entry( ){ }
    Entry( K k, V v ):key(k), value(v){ }   //构造函数
    Entry( Entry<K, V> const & e ):key(e.key), value(e.value){ }   //复制构造函数
    //比较器，判等器（从此不必严格区分词条极其对应的关键码）
    bool operator < ( Entry<K, V> const & e ){ return key < e.key; }
    bool operator > ( Entry<K, V> const & e ){ return key > e.key; }
    bool operator == ( Entry<K, V> const & e ){ return key == e.key; }
    bool operator != ( Entry<K, V> const & e ){ return key != e.key; }
    ~Entry(){}
};

template<typename T>
class BST: public BinTree<T>{   //注意：继承了 Bintree<T>
    public:
        BST(){ }
        BST( BinNode<T>* root ):BinTree<T>( root ){ }
        ~BST(){}        

        //虚函数，便于子类重写
        virtual BinNode<T>* & search(const T & );   //搜索，查找
        virtual BinNode<T>* & searchIn(BinNode<T>** v, const T & e, BinNode<T>* & hot );
        
        virtual BinNode<T>* & insert(const T & );   //插入
        virtual bool remove(const T & );   //按数据内容删除
        virtual BinNode<T>* removeAt( BinNode<T>* x, BinNode<T>* & hot );   //按位置删除
    protected:
        BinNode<T>* _hot;   //被命中节点的父亲,与查找算法配合（或许直接返回一个 pair 更好）
        
        BinNode<T>* connect34(   // “ 3 + 4 ”重构
            BinNode<T>* a, BinNode<T>* b, BinNode<T>* c,
            BinNode<T>* T0, BinNode<T>* T1, BinNode<T>* T2, BinNode<T>* T3
        );
        BinNode<T>* rotateAt(BinNode<T>* g );   //旋转调整
        void up_link( BinNode<T>* g, BinNode<T>* b );   //向上连接，传入祖父和新的局部子树的根节点
};

template<typename T>
BinNode<T>* BST<T>::rotateAt(BinNode<T>* v ){   //返回局部子树的根节点
    BinNode<T>* p = v->parent;   //得到父亲
    BinNode<T>* g = p->parent;   //得到祖父,失衡时祖父不可能为空
    if( p == g->lChild ){
        if( v == p->lChild ){   // 2、
            up_link( g, p );
            return connect34( v, p, g, v->lChild, v->rChild, p->rChild, g->rChild );
        }else{   // 3、
            up_link( g, v );
            return connect34( p, v, g, p->lChild, v->lChild, v->rChild, g->rChild );
        }
    }else{
        if( v == p->rChild ){   //  1、
            up_link( g, p );
            return connect34( g, p, v, g->lChild, p->lChild, v->lChild, v->rChild );
        }else{   //  4、
            up_link( g, v );
            return connect34( g, v, p, g->lChild, v->lChild, v->rChild, p->rChild );
        }
    }
}

template<typename T>
void BST<T>::up_link( BinNode<T>* g, BinNode<T>* b ){   //向上连接
    BinNode<T>* r_p = g->parent;   //局部子树根节点的父亲
    b->parent = r_p;   //注意一定要把局部子树的根节点连到整棵树上
    if( r_p ){
        b == r_p->lChild ? r_p->lChild = b : r_p->rChild = b;
    }
    if( g == BinTree<T>::_root ){   //考虑 g 为全树根节点时的更新
        BinTree<T>::_root = b; 
    }
    return ;
}

template<typename T>
BinNode<T> * BST<T>::connect34( BinNode<T> * a, BinNode<T> * b, BinNode<T> * c, 
                        BinNode<T> * T0, BinNode<T> * T1, BinNode<T> * T2, BinNode<T> * T3 ){
    a->lChild = T0; if( T0 ){ T0->parent = a; }   //注意要判断是否为空
    a->rChild = T1; if( T1 ){ T1->parent = a; } this->updateHeight( a );   //更新高度（a 的高度）
    c->lChild = T2; if( T2 ){ T2->parent = c; }
    c->rChild = T3; if( T3 ){ T3->parent = c; } this->updateHeight( c );   //更新高度（c 的高度）
    b->lChild = a; a->parent = b;
    b->rChild = c; c->parent = b; this->updateHeight( b );   //更新高度（b 的高度）
    return b;   //返回局部子树的根节点
}

template<typename T>
BinNode<T>* & BST<T>::search(const T & e ){
    return searchIn( & (BinTree<T>::_root), e, _hot = nullptr );   //从根节点开始查找，关键码为 e，_hot 指向被命中节点的父亲  
}   //注意：上方 _hot 一定要赋空值，一是为了更新数值，而是为了避免“第一个插入”的边界情况


//如果下方的形参 v 是 BinNode<T>* & 类型，且传入 _root，那么 _root 的值就会被改变，从而造成错误
template<typename T>
BinNode<T>* & BST<T>::searchIn(BinNode<T>** v, const T & e, BinNode<T>* & hot ){
    while( *v != nullptr && (*v)->data != e ){   //查找完毕或当前节点不是命中节点
        hot = *v;   //记录命中节点的父亲
        v = ( e < (*v)->data ) ? & (*v)->lChild : & (*v)->rChild;   //二分查找，深入
    }
    return *v;
//返回值为:一个指向欲查找节点位置的节点的左指针或右指针的引用
}       //注意不能返回局部变量的引用。这里使用了二级指针，因为最后要返回一个指针的引用，即要返回一个指针本身，而不是指针所指

template<typename T>
BinNode<T>* & BST<T>::insert(const T & e ){
    BinNode<T>* & x = search( e );   //得到待插入的位置
    if( !x ){   //因为假设没有重复的元素，所以在查找失败时再插入(即 x 为空时)
        x = new BinNode<T>( e, _hot );   //建立节点, _hot在上方 search 中发生了改变
        BinTree<T>::_size++;
        BinTree<T>::updateHeightAbove( x );
    }
    return x;   //无论 e 是否存在于原树中，都有 x->data == e
}

template<typename T>  //按照位置删除          // hot 记录了搜索后待删除节点的父亲
BinNode<T>* BST<T>::removeAt( BinNode<T>* x, BinNode<T>* & hot ){   //返回待删除节点的接替节点(不是固定返回中序后继)
    BinNode<T>* succ = nullptr;   //指向待删除节点的接替者
    hot = x->parent;   //记录被删除节点的父节点
    if( !x->lChild ){   //如果左为空   //只要有一个为空（叶子节点也符合条件）
        succ = x->rChild;
        if( succ != nullptr ){   //右孩子不为空
            succ->parent = hot;
        }    //右孩子为空
    }else if( !x->rChild ){   //如果此时右为空,左孩子肯定存在
        succ = x->lChild;
        succ->parent = hot;
    }else{   //有两个孩子，succ 不可能为空
        succ = x->succ();
        //succ->parent = x;
        std::swap( x->data, succ->data );   //交换，可交换数据，也可进行指针移动
                                    //根据中序遍历的大小顺序，此时新的待删除节点的左孩子一定为空，对其按照左为空的方式删除即可
        x = succ;   //转移指针指向，x 仍指向需删除的节点
        hot = x->parent;   //转移 hot 指向，保持语义
        succ = succ->rChild;     //注意：重复左为空时的操作
        if( succ != nullptr ){
            succ->parent = hot;
        }
    }
    if( hot != nullptr ){   // hot 不为空，即待删除节点不是根节点
        ( hot->lChild == x ) ? hot->lChild = succ : hot->rChild = succ;
    }else{
        BinTree<T>::_root = succ;
    }
    //delete x->data;   要考虑数据域是否需要释放
    delete x;   //注意 x 的语义，注意它与搜索算法的返回值的联系，以避免产生野指针
    BinTree<T>::_size--;
    BinTree<T>::updateHeightAbove( hot );   //从 hot 开始更新高度
    return succ;
}

template<typename T>
bool BST<T>::remove(const T & e ){   //按照数据内容内容删除
    BinNode<T>* x = search( e );  //定位,注意这是一个指针引用，注意 _hot 变量记录了父节点
    if( x == nullptr ){   //是否存在该节点
        return false;
    }
    removeAt( x, _hot );   //借用
    return true;
}

#endif