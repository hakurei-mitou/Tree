#ifndef BINTREE_HEAD
#define BINTREE_HEAD
#include<algorithm>

//注意：未严格封装

//注意：为了简化，有些函数列出声明并做空处理，知道其原理即可

#define stature(p) ( (p) ? (p)->height : -1 )   //统一高度语义

template<typename T>
class BinNode{
    public:
        BinNode<T> * parent, * lChild, * rChild;   //注意这里的 * 号
        T data;
        int height;   //高度
        int npl;      //npl 值，（左式堆）
        int color;   //颜色，（红黑树）

        BinNode();
        BinNode( T data, BinNode<T>* parent ){   //构造以 parent 为父节点的一个 叶子 节点，若只允许二叉树两个子节点则应当做判断
            this->data = data;   //构造单个节点将 parent 参数作为 nullptr 即可
            this->parent = parent;
            lChild = rChild = nullptr;
            height = 0;
            npl = 0;
            color = 0;
        }
        
        int size();   //子树规模
        //注意：以下两个插入函数只是为了辅助树类插入节点
        BinNode<T>* insertAsLC(T const & );   //作为左孩子插入新节点，注意，这里的参数是数据，不是一个节点类
        BinNode<T>* insertAsRC(T const & );   //作为右孩子插入新节点
        BinNode<T>* succ();   //中序遍历下，当前节点的直接后继，即 BST 中不小于当前节点的最小节点。约定：最后一个节点的中序后继为空

/*   为简化，做空处理        
        template<typename VST> void travLevel(VST & );   //子树层次遍历
        template<typename VST> void travPre(VST & );   //子树先序遍历
        template<typename VST> void travIn(VST & );   //子树中序遍历
        template<typename VST> void travPost(VST & );   //子树后序遍历
*/
        ~BinNode(){}
};

template<typename T>
class BinTree{   //注意区分哪些是节点类接口，哪些是树类接口
    protected:
        int _size;
        BinNode<T>* _root;
        
        virtual int updateHeight(BinNode<T>* x );   //更新节点 x 的高度，不同二叉树高度定义可能不同，所以设为虚函数以便覆盖
        virtual void updateHeightAbove(BinNode<T>* x );   //更新 x 及其祖先的高度
    
    public:
        BinTree( BinNode<T>* root ){   //将该节点作为根节点建立树
            _size = 1;
            _root = root;
        }
        BinTree(){
            _size = 0;
            _root = nullptr;
        }
        BinNode<T>* get_root(){
            return _root;
        }
        BinNode<T>* insertAsRC( BinNode<T>* x, T const & e );   // insertAsLC 与其对称
        BinNode<T>* insertAsLC( BinNode<T>* x, T const & e );   // 返回被插入节点的位置
        int size() const { return _size;}   //规模
        bool empty() const { return ! _root;}   //判空
        BinNode<T>* root() const { return _root;}   //根节点
        //子树插入，删除，和分离接口
        //遍历接口
        ~BinTree(){
            delete_all_BinTree( _root );
        }
        void delete_all_BinTree(BinNode<T>* v );   //以后序遍历模式删除子树
};

//注意区分哪些是节点类接口，哪些是树类接口

template<typename T>
BinNode<T>* BinNode<T>::insertAsLC(T const & e ){
    return lChild = new BinNode( e, this );
}

template<typename T>
BinNode<T>* BinNode<T>::insertAsRC(T const & e ){
    return rChild = new BinNode( e, this );
}

template<typename T>
int BinNode<T>::size(){
    int count = 1;
    if( lChild ){
        count += lChild->size();   //递归计入左子树规模
    }
    if( lChild ){
        count += rChild->size();   //递归计入右子树规模
    }
    return count;
}

template<typename T>   //中序遍历下，当前节点的直接后继，即 BST 中不小于当前节点的最小节点。约定：最后一个节点的中序后继为空
BinNode<T>* BinNode<T>::succ(){
    BinNode<T>* t = this->rChild;
    if( t != nullptr ){   //若右孩子不为空
        while( t->lChild ){   //往左深入，最左边的节点就是所求节点；若右孩子的左节点为空，则所求节点就是右孩子
            t = t->lChild;
        }
        return t;
    }
    t = this;   //若右孩子为空
    BinNode<T>* p = this->parent;
    while( p != nullptr && t != p->lChild ){   //向祖先追溯，直到“当前节点（t）”是其父节点的左节点时，其父节点就是要求的后继
        p = p->parent;                          //若追溯到根节点还是没符合要求，即 p 为空时，代表当前节点为中序遍历的最后一个节点
        t = t->parent;
    }
    return p;
}

template<typename T>
int BinTree<T>::updateHeight(BinNode<T>* x ){
    return x->height = 1 + std::max( stature( x->lChild ), stature( x->rChild ) );
}

template<typename T>
void BinTree<T>::updateHeightAbove(BinNode<T>* x ){
    if( x ){
        int t = x->height;
        while( x != nullptr && x->height <= t+1 ){   //优化：一但父亲高度不会变化就可终止，没有必要一直追溯到根节点
            t = updateHeight( x );   //更新并记录
            x = x->parent;   //得到其父亲
        }
    }
    return ;
}

template<typename T>
BinNode<T>* BinTree<T>::insertAsRC( BinNode<T>* x, T const & e ){   // insertAsLC 与其对称
    _size++;
    x->insertAsRC( e );   //   注意：                  节点的接口调用
    updateHeightAbove( x );   //更新自己和祖先高度
    return x->rChild;
}

template<typename T>
BinNode<T>* BinTree<T>::insertAsLC( BinNode<T>* x, T const & e ){
    _size++;
    x->insertAsLC( e );   //   注意：                  节点的接口调用
    updateHeightAbove( x );   //祖先高度可能增加
    return x->lChild;
}

template<typename T>
void BinTree<T>::delete_all_BinTree(BinNode<T>* v ){
    if( v == nullptr ){
        return ;
    }
    delete_all_BinTree( v->lChild );
    delete_all_BinTree( v->rChild );
    delete v;
    _size--;
}

#endif