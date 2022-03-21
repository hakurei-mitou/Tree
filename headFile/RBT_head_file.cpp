#ifndef RBT_HEAD
#define RBT_HEAD

#include"BST_head_file.cpp"

enum color {Red, Black};   //Red, Black

template<typename T>
class RBT: public BST<T> {
    public:
        BinNode<T>* & insert(const T & e);
        bool remove(const T & e);
    protected:
        void solveDoubleRed(BinNode<T>* x);
        //void solveRedBlack(BinNode<T>* x);   //一红一黑较简单，故直接在 remove 中处理
        void solveDoubleBlack(BinNode<T>* r);
        void BB1(BinNode<T>* s, BinNode<T>* p, BinNode<T>* t);   //辅助函数
        int updateHeight(BinNode<T>* x);
};

template<typename T>
int RBT<T>::updateHeight(BinNode<T>* x) {   //黑高度（黑深度）
    x->height = std::max(stature(x->lChild), stature(x->rChild));
    if (x->color == Black) {
        ++(x->height);
    }
    return x->height;
}

template<typename T>
BinNode<T>* & RBT<T>::insert(const T & e) {   //假定没有重复元素
    BinNode<T>* & x = BST<T>::search(e);
    if (x) {   //若 x 已存在 
        return x;
    }
    //注意插入默认染红
    x = new BinNode<T>(e, BST<T>::_hot);   //在该位置创建初始状态节点（一个红节点），黑高度初始化为 -1
    x->color = Red;
    x->height = -1;
    ++RBT<T>::_size;
    RBT<T>::solveDoubleRed(x);
    return x;   //返回插入的节点
}

template<typename T>
void RBT<T>::solveDoubleRed(BinNode<T>* x ) {
    BinNode<T>* p = x->parent;
    if (!p) {   //如果 p 为空，即 x 为根节点
        x->color = Black;   //染为黑色后退出即可
        updateHeight(x);   //注意先后
        return;
    }
    while (x->color == Red && p->color == Red) {   //是否存在双红缺陷   //注意短路原则，下方 RR-1 和 RR-2 都有用到
        BinNode<T>* g = p->parent;   //存在双红缺陷则 g 必定不为空
        BinNode<T>* u = g->rChild != p ? g->rChild : g->lChild;   //得到叔父（父亲的兄弟）
        if (u == nullptr || u->color == Black) {   //RR-1
            if (g->lChild == p) {
                if (p->lChild == x) {   //图示中的 a
                    BST<T>::up_link(g, p);   //向上连接
                    x->color = Red; p->color = Black; g->color = Red;   //重染色
                    BST<T>::connect34(x, p, g, x->lChild, x->rChild, p->rChild, g->rChild);   //重构
                    //重构内是含有更新高度的操作的
                } else {   //p->rChild == x，图示中的 b
                    BST<T>::up_link(g, x);
                    p->color = Red; x->color = Black; g->color = Red;   //重染色
                    BST<T>::connect34(p, x, g, p->lChild, x->lChild, x->rChild, g->rChild);
                }
            } else {   //p->lChild == u，对称的两种情况
                if (p->lChild == x) {
                    BST<T>::up_link(g, x);
                    g->color = Red; x->color = Black; p->color = Red;   //重染色
                    BST<T>::connect34(g, x, p, g->lChild, x->lChild, x->rChild, p->rChild);
                } else {
                    BST<T>::up_link(g, p);
                    g->color = Red; p->color = Black; x->color = Red;   //重染色
                    BST<T>::connect34(g, p, x, g->lChild, p->lChild, x->lChild, x->rChild);
                }             
            }
        } else {   //u->color == Red   //RR-2
            p->color = Black; g->color = Red; u->color = Black;   //统一 p u 染黑，g 染红即可
            updateHeight(p);
            updateHeight(u);
            x = g;   //上升的 g 相当于成了新的插入节点 x
            g = g->parent;   //如果 g 此时上升到了根节点，则必定会结束缺陷。因短路原则，不必担心上方的循环判断条件解引用空指针
            if (x->parent == nullptr) {   //如果新的 x（即原来的 g）是根节点
                x->color = Black;
            }
            updateHeight(x);
        }
    }
    return;
}

template<typename T>
bool RBT<T>::remove(const T & e) {
    BinNode<T>* x = BST<T>::search(e);
    if (!x) {   //若 x 不存在 
        return false;
    } else {
        if (x->parent != nullptr && x->lChild == nullptr && x->rChild == nullptr) {   //叶子特殊处理
            std::swap(x->data, x->parent->data);
            x = x->parent;
        }
        auto t = x->color;
        BinNode<T>* r = BST<T>::removeAt(x, BST<T>::_hot);
        //注意：这个删除算法不会返回空的后继，除非删除的是中序遍历的最后一个节点或其为叶子（返回了空的右孩子）
        //删除算法内部含有更新高度的函数
        //这个删除算法在待删除节点有两个孩子时执行的交换只是交换数据，并未交换颜色
        BinNode<T> * p = BST<T>::_hot;
        if (r == nullptr && p != nullptr) {   // 后继为空特殊处理
            if (p->color == Red) {
                p->color = Black;
            }
            if (p->lChild != nullptr) {
                p->lChild->color = Red;
                updateHeight(p->lChild);
            }
            if (p->rChild != nullptr) {
                p->rChild->color = Red;
                updateHeight(p->rChild);
            }
            updateHeight(p);
            return true;
        } else if (r->color != t) {   //RB 一红一黑
            r->color = Black;   // removeAt 会处理 x 是根节点的特殊情况
            updateHeight(r);
        } else {   //双黑
            solveDoubleBlack(r);   //传入后继
        }
    }
    return true;
}

template<typename T>
void RBT<T>::solveDoubleBlack(BinNode<T>* r) {
    BinNode<T>* p = r->parent;
    if (p == nullptr) {   //到根
        return;
    }
    BinNode<T>* s = (r != p->lChild ? p->lChild : p->rChild); 
    if (s->color == Black) {
        if (s->lChild->color == Red || s->rChild->color == Red) {   //BB-1
            BinNode<T>* t = (s->lChild->color == Red ? s->lChild : s->rChild);   //取一个红的 t
            BB1(s, p, t);
        } else /*if ( s->lChild->color == Black && s->rChild->color == Black)*/ {
            if (p->color == Red) {   //BB-2-R
                std::swap(s->color, p->color);
            } else {   //BB-2-B
                s->color = Red;
                solveDoubleBlack(p);   // p 成了新的 “x” 的 r
            }
        }
    } else {   //BB-3
        std::swap(s->color, p->color);
        BinNode<T>* g = p->parent;
        //向上联结
        if (g == nullptr) {   // 特殊情况，若 p 此时为根
            BinTree<T>::_root = s;
            s->parent = nullptr;
        } else {
            s->parent = g;
            p == g->lChild ? g->lChild = s : g->rChild = s;
        }
        if (s == p->lChild) {   //zig
            p->lChild = s->rChild;
            s = p->lChild;   //新的 s
        } else {   //zag
            p->rChild = s->lChild;
            s = p->rChild;   //新的 s
        }
        if (s->lChild->color == Red || s->rChild->color == Red) {   //BB-1
            BinNode<T>* t = (s->lChild->color == Red ? s->lChild : s->rChild);   //取一个红的 t
            BB1(s, p, t);
        } else {   //BB-2-R
            std::swap(s->color, p->color);   //此时 p 一定为红
            updateHeight(p);
            updateHeight(s);
        }
    }
    return;
}

template<typename T>
void RBT<T>::BB1(BinNode<T>* s,BinNode<T>* p, BinNode<T>* t) {
    //根据局部子树的不同情况进行重构和染色，高度更新在重构函数内
    if (p->lChild == s) {
        if (s->lChild == t) {
            t->color = Black; s->color = p->color; p->color = Black;
            BST<T>::connect34(t, s, p, t->lChild, t->rChild, s->rChild, p->rChild);
        } else {
            s->color = Black; t->color = p->color; p->color = Black;
            BST<T>::connect34(s, t, p, s->lChild, t->lChild, t->rChild, p->rChild);
        }
    } else {
        if (s->lChild == t) {
            t->color = p->color; p->color = Black; s->color = Black;   //不要在继承 p 颜色前更改 p 的颜色
            BST<T>::connect34(p, t, s, p->lChild, t->rChild, t->rChild, s->rChild);
        } else {
            s->color = p->color; p->color = Black;  t->color = Black;
            BST<T>::connect34(p, s, t, p->lChild, s->lChild, s->rChild, t->rChild);
        }
    }
    return;
}

#endif