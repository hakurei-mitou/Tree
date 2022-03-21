#ifndef BT_HEAD
#define BT_HEAD
#include<vector>
#include<algorithm>

template<typename T>
class BTNode{
    public:
        BTNode<T>* parent;
        std::vector<T> key;
        std::vector< BTNode<T>* > child;

        BTNode(){ parent = nullptr; }
        BTNode( T e, BTNode<T>* lc = nullptr, BTNode<T>* rc = nullptr ){
            parent = nullptr;
            key.insert( key.begin(), e );
            child.insert( child.begin(), lc );   //两个孩子
            child.insert( child.begin()+ 1, rc );
            if( lc ){
                lc->parent = this;
            }
            if( rc ){
                rc->parent = this;
            }
        }
        ~BTNode(){ }

        int search_help( const T e );   //在超级节点内的查找函数
        
        //数据转移函数，将数据从 from 的 key中下标 a 至末尾的数据，移动到 to 的下标 b 至末尾；child 将一起处理
        static void data_move( BTNode<T>* from, int a, BTNode<T>* to, int b );
};

template<typename T>
class BT{
    protected:
        int _size = 0;   //规模（即关键码总数）
        int _order;   //阶次
        BTNode<T>* _root;   //根节点
        BTNode<T>* _hot;   //存放辅助结点，存放 search 最后访问的非空节点位置
        int _rank = -1;   //辅助变量，，存放 search 找到的关键码数组对应的 秩（即下标）,没找到为 -1
        void solveOverflow( BTNode<T>* v );   //因插入而上溢的分裂处理
        void solveUnderflow( BTNode<T>* v );   //因删除而下溢的旋转及合并处理
    public:
        BT(){ 
            _size = 0; _order = 0; _root = _hot = nullptr; _rank = -1;
        }
        BT( int order, T root_e ){   //阶次，根节点数据
            _root = new BTNode( root_e );
            _order = order;
            _size = 1;
            _hot = nullptr;
            _rank = -1;
        }
        ~BT(){ }
        BTNode<T>* get_root(){
            return _root;
        }
        BTNode<T>* search( const T & e );   //搜索
        bool insert( const T & e );   //插入
        bool remove( const T & e );   //删除
};

template<typename T>
int BTNode<T>::search_help( const T e ){   //返回不大于 e 的最后一个关键码位置，若第一个关键码就大于 e，则返回 -1
    int i = 0;
    for( ; i < (int)this->key.size(); i++ ){
        if( this->key[i] > e ){
            break;
        }
    }
    return i-1;
}

template<typename T>
void BTNode<T>::data_move( BTNode<T>* from, int a, BTNode<T>* to, int b ){
    int i = a;
    while( i < (int)from->key.size() ){   //转移数据
        to->key.insert( to->key.begin()+ b, from->key[ i ] );
        to->child.insert( to->child.begin()+ b, from->child[ i+1 ] );
        i++, b++;
    }
    to->child.insert( to->child.begin()+ b, from->child[ i+1 ] );   //最后一个孩子
    from->key.erase( from->key.begin()+ a, from->key.end() );   //删除原始数据
    from->child.erase( from->child.begin()+ a+1, from->child.end() );   //从下标为 a+1 的孩子开始
    return ;
}

template<typename T>
BTNode<T>* BT<T>::search( const T & e ){   //查找
    BTNode<T>* v = _root;
    _hot = nullptr;
    while( v ){   //不断深入
        _rank = v->search_help( e );
        if( _rank < 0 || v->key[ _rank ] != e ){   //注意这个判断条件，注意 _rank 为 -1 时的情况
            _hot = v;   //当前节点未找到，将当前节点记录为父节点，到下一个节点查找
            v = v->child[ _rank + 1 ];   //到下一个节点，注意 _rank+1 为超尾值时代表什么
        }else{
            return v;   //找到关键码，返回当前节点
        }
    }
    return nullptr;   //到达外部节点
}

template<typename T>
bool BT<T>::insert( const T & e ){
    BTNode<T>* v = search( e );
    if( v ){   //如果 e 存在，则插入失败
        return false;
    }
    _hot->key.insert( _hot->key.begin()+_rank + 1, e );   //注意下标，插入到不大于 e 的最后一个节点的下一个位置
    _hot->child.insert( _hot->child.begin()+_rank + 2, nullptr );
    _size++;   //更新规模
    solveOverflow( _hot );   //插入可能上溢
    return true;
}

template<typename T>
void BT<T>::solveOverflow( BTNode<T>* v ){
    while( (int)v->child.size() > _order ){   //若分支数大于阶次
        BTNode<T>* p = v->parent;      //得到其父亲
        int mid = v->key.size() / 2;   //中位数
        T temp_key = v->key[ mid ];   //备份数据
                                    //BTNode<T>* temp_child = v->child[ mid ];   //这个孩子不必移动
        v->key.erase( v->key.begin()+mid );           //删除数据
        BTNode<T>* split = new BTNode<T>();   //新建分裂节点
        if( !p ){   //如果是根节点
            p = new BTNode<T>( temp_key, v, split );   //建立新的根节点
            _root = p;
        }else{   //不是根节点
            int i = p->search_help( temp_key );
            p->key.insert( p->key.begin()+i + 1, temp_key );
            p->child.insert( p->child.begin()+i + 2, split );   //孩子，连接。 v 不需要连接
            split->parent = p;   //这一句是给 p 不为空时准备的，p 为空时，根节点的构造函数已经连接好了 parent
        }
        BTNode<T>::data_move( v, mid, split, 0 );   //转移数据
        if( _root == p ){    //向上延伸
            break;
        }else{
            v = p;
        }
    }
    return ;
}

template<typename T>
bool BT<T>::remove( const T & e ){
    BTNode<T>* v = search( e );
    if( !v ){   //若不存在则返回 false
        return false;
    }
    if( ! v->child[ _rank ] ){   // 1、2、
        if( ! v->parent && v->key.size() <= 1  ){   //若为删除根节点最后一个关键码的特殊情况
            if( ! v->child[1] ){   //如果有右孩子
                _root = v->child[1];   //直接将右孩子变为根节点
                delete v;
                v = _root;
            }else{   //如果没有右孩子，则全树只剩这一个节点，一个关键码，全树空
                v->key.clear();
                v->child.pop_back();
                // v->child[0] = nullptr;   要留个空指针，以便后续插入
                _size--;    //因为提早返回了，所以这里要更新
                return true;   //提早返回，避免对空的根节点调用下溢函数
            }
        }else{   //一般情况
            v->key.erase( v->key.begin()+ _rank );
            v->child.erase( v->child.begin()+ _rank );
        }
    }else{
        if( ! v->child[ _rank ] ){   // 3、
            if( ! v->parent && v->key.size() <= 1  ){   //若为删除根节点最后一个关键码的特殊情况
                _root = v->child[0];   //直接将左孩子变为根节点
                delete v;
                v = _root;
            }else{   //一般情况
                v->key.erase( v->key.begin()+ _rank );
                v->child.erase( v->child.begin()+ _rank );
            }
        }else{   // 4、
            BTNode<T>* u = v->child[ _rank+1 ];
            //if( u )   往右子树的左方不断延伸，此时右子树不可能为空
            while( u->child[0] ){   //注意这里的下标
                u = u->child[0];
            }
            std::swap( v->key[ _rank ], u->key[0] );
            v = u;   //转移指向
            _rank = 0;   //转移记录的下标值，便于处理下溢
               //一般情况,特殊情况均可行:
            v->key.erase( v->key.begin() );   //按照左孩子为空的方式处理:
            v->child.erase( v->child.begin() );
        }
    }
    _size--;   //更新规模。   对于根节点特殊情况，这里已经更新了规模，下溢函数不要再更新
    solveUnderflow( v );
    return true;
}

template<typename T>
void BT<T>::solveUnderflow( BTNode<T>* v ){   //注意各下标
    int under =  (_order + 1) / 2;   //下界
    BTNode<T>* p = v->parent;
    if( ! p ){   //根节点下溢，忽略,只有删除根节点最后一个关键码时才作处理
        return ;
    }
    if( (int)v->child.size() < under ){   //若分支数小于 阶次的一半向上取整
        BTNode<T> * left_bother = nullptr, * right_bother = nullptr;   //左边和右边的兄弟
        int p_rank = p->search_help( v->key[0] );   //取当前节点中任意一个节点确定父节点中指向当前节点的关键码位置，这里取下标为 0 的关键码
        if( p_rank != -1 ){   // p_rank 可能为 -1,因为若 p 的第一个关键码就大于它，search_help 函数返回 -1
                                            //此时 v 肯定不存在左边的兄弟，在旋转的第一个if中向量取 -1 的下标会出错
        // 旋转：
            if( (left_bother = p->child[ p_rank ]) ){   //如果存在左边的兄弟，判断其是否会因为旋转导致进一步下溢
                if( (int)left_bother->child.size() - 1 >= under  ){   //不会导致进一步下溢
                    v->key.insert( v->key.begin(), p->key[ p_rank ] );
                    v->child.insert( v->child.begin(), p->child[ p_rank ] );
                    p->key[ p_rank ] = left_bother->key.back();
                    p->child[ p_rank ] = left_bother->child.back();
                    left_bother->key.pop_back();
                    left_bother->child.pop_back();
                    return ;
                }
            }
        }
        if( p_rank + 2 <= (int)p->child.size() - 1 && (right_bother = p->child[ p_rank + 2 ]) ){   //如果存在右边的兄弟
            if( (int)right_bother->child.size() - 1 >= under ){
                v->key.push_back( p->key[ p_rank + 1 ] );
                v->child.push_back( right_bother->child.front() );
                p->key[ p_rank + 1 ] = right_bother->key.front();
                right_bother->key.erase( right_bother->key.begin() );
                right_bother->child.erase( right_bother->child.begin() );
                return ;
            }
        }
        //上方都不能旋转则只有下方     合并
        if( p == _root && (int)p->key.size() < 2 ){   //如果合并到根节点，且根节点只含一个关键码
            if( left_bother ){   //左不为空
                left_bother->key.push_back( p->key[0] );
                BTNode<T>::data_move( v, 0, left_bother, left_bother->key.size() );
                delete p;
                _root = left_bother;
                return ;
            }else{
                v->key.push_back( p->key[0] );
                v->child.push_back( p->child[1] );
                delete p;
                _root = v;
                return ;
            }
        }else{
            if( left_bother ){   //左不为空
                left_bother->key.push_back( p->key[ p_rank ] );
                p->key.erase( p->key.begin()+ p_rank );
                p->child.erase( p->child.begin()+ p_rank + 1 );
                BTNode<T>::data_move( v, 0, left_bother, left_bother->key.size() );
                solveUnderflow( p );
            }else{   //左为空,则把 v 变为其左孩子
                v->key.push_back( p->key[ p_rank ] );
                v->child.push_back( nullptr );
                p->key.erase( p->key.begin()+ p_rank );
                p->child.erase( p->child.begin()+ p_rank );   //此时 v 变为了 p 的左孩子
                solveUnderflow( p );
            }
        }
    }
    return ;
}

#endif