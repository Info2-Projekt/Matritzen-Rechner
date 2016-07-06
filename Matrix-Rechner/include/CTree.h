
#ifndef __CTREE_H
#define __CTREE_H

#include <iostream>
#include <deque>


namespace Codebook {

template<typename Traits>
class CTree {
public:
  typedef typename CTree<Traits> my_type;
  typedef typename Traits::value_type value_type;
  typedef typename Traits::key_type key_type;
  typedef unsigned long size_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;


public:

//*********************************************************
// CKnot
//*********************************************************
  class CKnot {
  public:
    CKnot *m_father;
    CKnot *m_left;
    CKnot *m_right;
    value_type m_data;

    CKnot(CKnot *fa, CKnot *li, CKnot *re)
    :m_father(fa),m_left(li),m_right(re)
    { }

    CKnot(CKnot *fa, CKnot *li, CKnot *re, const value_type &v)
    :m_father(fa),m_left(li),m_right(re),m_data(v)
    {}

    virtual ~CKnot()
    {}
  };


protected:
  CKnot *m_root;
  size_type m_size;


  void deleteKnot(CKnot *kn) {
    if(kn) {
      if(kn->m_left) deleteKnot(kn->m_left);
      if(kn->m_right) deleteKnot(kn->m_right);
      delete(kn);
    }
  }


//*********
  CKnot *symmetricPred(CKnot *kn) const {
    CKnot *cur=kn->m_left;
    if(!cur)
      return(0);
    while(cur->m_right)
      cur=cur->m_right;
    return(cur);

  }

//*********
  CKnot *symmetricSucc(CKnot *kn) const {
    CKnot *cur=kn->m_right;
    if(!cur)
      return(0);
    while(cur->m_left)
      cur=cur->m_left;
    return(cur);
  }


//*********
  CKnot *findFirstKnot(const key_type &k) const{
/*
** Baum leer?
** => Knoten nicht gefunden
*/
    if(!m_root) return(0);
/*
** Obersten Knoten mit Schlüssel gleich k suchen
*/
    CKnot *cur=m_root;
    while(cur&&(Traits::ne(Traits::getKey(cur->m_data),k))) {
      if(Traits::lt(k,Traits::getKey(cur->m_data)))
        cur=cur->m_left;
      else
        cur=cur->m_right;
    }
/*
** Um ersten Knoten mit Schlüssel gleich k zu finden
** Solange im linken Ast hinabsteigen wie Schlüssel gleich k
*/
    while((cur)&&(cur->m_left)&&(Traits::eq(Traits::getKey(cur->m_left->m_data),k)))
      cur=cur->m_left;
/*
** Überprüfen, ob symmetrische Vorgänge mit Schküssel
** gleich k existieren.
*/
    if(cur) {
      CKnot *pred=symmetricPred(cur);
      while((pred)&&(Traits::eq(Traits::getKey(pred->m_data),k))) {
        cur=pred;
        pred=symmetricPred(cur);
      }
    }

    return(cur);
  }

//*********
  CKnot *findLastKnot(const key_type &k) const{
/*
** Baum leer?
** => Knoten nicht gefunden
*/
    if(!m_root) return(0);
/*
** Obersten Knoten mit Schlüssel gleich k suchen
*/
    CKnot *cur=m_root;
    while(cur&&(Traits::ne(Traits::getKey(cur->m_data),k))) {
      if(Traits::lt(k,Traits::getKey(cur->m_data)))
        cur=cur->m_left;
      else
        cur=cur->m_right;
    }
/*
** Um letzten Knoten mit Schlüssel gleich k zu finden
** Solange im rechten Ast hinabsteigen wie Schlüssel gleich k
*/
    while((cur)&&(cur->m_right)&&(Traits::eq(Traits::getKey(cur->m_right->m_data),k)))
      cur=cur->m_right;
/*
** Überprüfen, ob symmetrische Nachfolger mit Schküssel
** gleich k existieren.
*/
    if(cur) {
      CKnot *succ=symmetricSucc(cur);
      while((succ)&&(Traits::eq(Traits::getKey(succ->m_data),k))) {
        cur=succ;
        succ=symmetricSucc(cur);
      }
    }
    return(cur);
  }

  
private:

  void copySons(CKnot* d, CKnot *s) {
    if(d&&s) {
/*
** Linken Sohn kopieren, falls vorhanden und
** für ihn copySons rekursiv aufrufen
*/
      if(s->m_left) {
        d->m_left=new CKnot(d,0,0,s->m_left->m_data);
        copySons(d->m_left,s->m_left);
      }
/*
** Rechten Sohn kopieren, falls vorhanden und
** für ihn copySons rekursiv aufrufen
*/
      if(s->m_right) {
        d->m_right=new CKnot(d,0,0,s->m_right->m_data);
        copySons(d->m_right,s->m_right);
      }
    }
  }
//*********
  bool erase(CKnot *cur) {
    if(!cur) return(false);

    CKnot *father=cur->m_father;

/*
** Zu löschender Knoten hat keine Söhne?
** => Kann problemlos gelöscht werden
*/
    if((!cur->m_left)&&(!cur->m_right)) {
/*
** Zu löschender Knoten ist die Wurzel?
** => Baum leer
*/
      if(cur==m_root) {
        m_root=0;
        delete(cur);
        m_size--;
        return(true);
      }
/*
** Zu löschender Knoten ist nicht die Wurzel?
** => Vater muss berücksichtigt werden
*/
      else {
/*
** Je nachdem, ob zu löschender Knoten der linke oder
** rechte Sohn des Vaters ist, muss entsprechender Sohn
** des Vaters auf 0 gesetzt werden
*/
        if(father->m_left==cur) {
          father->m_left=0;
        }
        else {
          father->m_right=0;
        }
        delete(cur);
        m_size--;
        return(true);
      }
    }
/*
** Besitzt zu löschender Knoten zwei Söhne?
** => Zu löschenden Knoten durch symmetrischen
**    Vorgänger ersetzen und symmetrischen Vorgänger
**    löschen
*/
    if((cur->m_left)&&(cur->m_right)) {
      CKnot *sys=symmetricPred(cur);
      cur->m_data=sys->m_data;
      return(erase(sys));
    }
/*
** Besitzt zu löschender Knoten nur einen Sohn?
** => Sohn des zu löschenden Knotens wird Sohn vom
** Vater des zu löschenden Knotens
*/
    CKnot *son;
    if(cur->m_left)
      son=cur->m_left;
    else
      son=cur->m_right;

    if(cur!=m_root) {
      son->m_father=father;
      if(father->m_left==cur) {
        father->m_left=son;
      }
      else {
        father->m_right=son;
      }
    }
/*
** Ist zu löschender Sohn die Wurzel?
** => Sohn des zu löschenden Knotens wird Wurzel
*/
    else {
      son->m_father=0;
      m_root=son;
    }
    delete(cur);
    m_size--;
    return(true);
  }

//*********
  void insert(CKnot *kn) {
/*
** Baum leer?
** => Einzufügender Knoten wird die Wurzel
*/
    if(!m_root) {
      m_root=kn;
      m_size++;
      return;
    }

    CKnot *cur=m_root;
    while(cur) {
      if(Traits::gt(Traits::getKey(cur->m_data), Traits::getKey(kn->m_data))) {
/*
** Einzufügender Knoten kleiner als aktueller Knoten?
** => Falls vorhanden, im linken Teilbaum nach Einfügeposition
**    suchen. Andernfalls wird einzufügender Knoten linker
**    Sohn des aktuellen Knotens
*/
        if(!cur->m_left) {
          cur->m_left=kn;
          kn->m_father=cur;
          m_size++;
          return;
        }
        else {
          cur=cur->m_left;
        }
      }
/*
** Einzufügender Knoten größer/gleich dem aktuellen Knoten?
** => Falls vorhanden, im rechten Teilbaum nach Einfügeposition
**    suchen. Andernfalls wird einzufügender Knoten rechter
**    Sohn des aktuellen Knotens
*/
      else {
        if(!cur->m_right) {
          cur->m_right=kn;
          kn->m_father=cur;
          m_size++;
          return;
        }
        else {
          cur=cur->m_right;
        }
      }
    }
  }

protected:

//
//*********************************************************
// _iterator
//*********************************************************
//
  class _iterator;
  friend class _iterator;
  class _iterator : public std::iterator<std::bidirectional_iterator_tag, value_type> {
  public:
    CKnot *m_knot;
    CKnot *m_begin;
    CTree *m_tree;

    _iterator()
      :m_knot(0), m_begin(0), m_tree(0)
    {}

//*********

    _iterator(CKnot *kn, CTree *tr, CKnot *be)
      : m_knot(kn), m_tree(tr), m_begin(be)
    {}

//*********

    bool isBegin() const {
      return((m_knot!=0)&&(m_knot==m_begin));
    }

//*********

    reference operator*() {
      return(m_knot->m_data);
    }

//*********

    pointer operator->() {
      return(&(m_knot->m_data));
    }

//*********

    bool operator==(_iterator &i) const {
      return(m_knot==i.m_knot);
    }

//*********

    bool operator!=(_iterator &i) const {
      return(m_knot!=i.m_knot);
    }

  };



//
//*********************************************************
// const_iterator
//*********************************************************
//
  class _const_iterator;
  friend class _const_iterator;
  class _const_iterator : public std::iterator<std::bidirectional_iterator_tag, value_type> {
  public:
    CKnot *m_knot;
    CKnot *m_begin;
    const CTree *m_tree;
  public:
    _const_iterator()
      :m_knot(0), m_begin(0), m_tree(0)
    {}

//*********

    _const_iterator(CKnot *kn, const CTree *tr, CKnot *be)
      : m_knot(kn), m_tree(tr), m_begin(be)
    {}

//*********

    bool isBegin() const {
      return((m_knot!=0)&&(m_knot==m_begin));
    }

//*********

    const_reference operator*() {
      return(m_knot->m_data);
    }

//*********

    const_pointer operator->() {
      return(&(m_knot->m_data));
    }

//*********

    bool operator==(_const_iterator &i) const {
      return(m_knot==i.m_knot);
    }

//*********

    bool operator!=(_const_iterator &i) const {
      return(m_knot!=i.m_knot);
    }

  };

public:

//
//*********************************************************
// inorder_iterator
//*********************************************************
//
  class inorder_iterator;
  friend class inorder_iterator;
  class inorder_iterator : public _iterator {
  public:
    inorder_iterator()
      : _iterator(0,0,0)
    {}

//*********

    inorder_iterator(CKnot *kn, CTree *tr, CKnot *be)
      : _iterator(kn,tr,be)
    {}

//*********

    inorder_iterator(const inorder_iterator &i){
      *this=i;
    }

//*********

    inorder_iterator(const _iterator &i){
      *this=i;
    }

//*********

    inorder_iterator &operator++() {
/*
** Ende bereits erreicht?
** => Nichts machen
*/
      if(!m_knot)
        return(*this);
/*
** Existiert rechter Sohn?
** => Den linken Ast des rechten Sohns
**    komplett hinabsteigen
*/
      if(m_knot->m_right) {
        m_knot=m_knot->m_right;
        while(m_knot->m_left)
          m_knot=m_knot->m_left;
      }
/*
** Existiert kein rechter Sohn?
** ==> So lange hinaufsteigen, bis aktueller Knoten
**     der linke Sohn des Vaters ist.
**     Der Vater ist der neue Knoten
*/
      else {
        CKnot *son;
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(m_knot->m_right==son));
      }
      return(*this);
    }

//*********

    inorder_iterator &operator--() {
/*
** Besitzt Iterator Endposition?
** => Letztes Element der Durchlauf-Reihenfolge bestimmen
*/
      if(!m_knot) {
        *this=m_tree->inorder_begin();
        m_knot=m_tree->m_root;
        if(m_knot) {
          while(m_knot->m_right)
            m_knot=m_knot->m_right;
        }
       return(*this);
      }
/*
** Besitzt Iterator Anfangsposition?
** => Nichts machen
*/
      if(m_knot==m_begin)
        return(*this);

/*
** Existiert linker Sohn?
** => Den rechten Ast des linken Sohns
**    komplett hinabsteigen
*/
      if(m_knot->m_left) {
        m_knot=m_knot->m_left;
        while(m_knot->m_right)
          m_knot=m_knot->m_right;
      }
/*
** Existiert kein linker Sohn?
** ==> So lange hinaufsteigen, bis aktueller Knoten
**     der rechte Sohn des Vaters ist.
**     Der Vater ist der neue Knoten
*/
      else {
        CKnot *son;
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(m_knot->m_left==son));
      }
      return(*this);
    }

//*********

    inorder_iterator operator++(int) {
      inorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    inorder_iterator operator--(int) {
      inorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    inorder_iterator &operator=(const inorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    inorder_iterator &operator=(const _iterator &i) {
      *this=i.m_tree->inorder_begin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }

  };

//
//*********************************************************
// postorder_iterator
//*********************************************************
//
  class postorder_iterator;
  friend class postorder_iterator;
  class postorder_iterator : public _iterator {
  public:
    postorder_iterator()
      : _iterator(0,0,0)
    {}

//*********

    postorder_iterator(CKnot *kn, CTree *tr, CKnot *be)
      : _iterator(kn,tr,be)
    {}

//*********

    postorder_iterator(const postorder_iterator &i){
      *this=i;
    }

//*********

    postorder_iterator(const _iterator &i){
      *this=i;
    }

//*********

    postorder_iterator &operator++() {
/*
** Ende bereits erreicht?
** => Nichts machen
*/
      if(!m_knot)
        return(*this);

/*
** Vater ermitteln
*/
      CKnot *son=m_knot;
      m_knot=m_knot->m_father;
      if(m_knot) {

/*
** Rechter Ast noch nicht abgearbeitet?
** => Rechten Ast so weit wie möglich hinabsteigen,
**    wobei linke Teiläste Priorität haben
*/
        if((m_knot->m_right)&&((m_knot->m_right!=son))) {
          m_knot=m_knot->m_right;
          while((m_knot->m_left)||(m_knot->m_right)) {
            if(m_knot->m_left)
              m_knot=m_knot->m_left;
            else
              m_knot=m_knot->m_right;
          }
        }
      }

      return(*this);
    }

//*********

    postorder_iterator &operator--() {
/*
** Besitzt Iterator Endposition?
** => Letztes Element der Durchlauf-Reihenfolge bestimmen
*/
      if(!m_knot) {
        *this=m_tree->postorder_begin();
        m_knot=m_tree->m_root;
       return(*this);
      }

/*
** Besitzt Iterator Anfangsposition?
** => Nichts machen
*/
      if(m_knot==m_begin)
        return(*this);

/*
** Rechter Ast noch nicht abgearbeitet?
** => Ersten Knoten des rechten Astes bestimmen
*/
      if(m_knot->m_right) {
        m_knot=m_knot->m_right;
      }

/*
** Rechten Ast, aber linken Ast noch nicht abgearbeitet?
** => Ersten Knoten des linken Astes bestimmen
*/
      else if(m_knot->m_left) {
        m_knot=m_knot->m_left;
      }

/*
** Beide Äste abgearbeitet?
** => aufsteigen und nächsten Teilast finden
*/
      else {
        CKnot * son;
/*
** Laufe solange, wie Iterator im rechten Ast aufsteigt und
** kein linker Ast verfügbar ist oder der Iterator im linken
** Ast aufsteigt
*/
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(((son==m_knot->m_right)&&(!m_knot->m_left) )||(son==m_knot->m_left)));
        if(m_knot)
          m_knot=m_knot->m_left;
      }

      return(*this);
    }

//*********

    postorder_iterator operator++(int) {
      postorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    postorder_iterator operator--(int) {
      postorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    postorder_iterator &operator=(const postorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    postorder_iterator &operator=(const _iterator &i) {
      *this=i.m_tree->postorder_begin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }

  };

//
//*********************************************************
// preorder_iterator
//*********************************************************
//
  class preorder_iterator;
  friend class preorder_iterator;
  class preorder_iterator : public _iterator {
  public:
    preorder_iterator()
      : _iterator(0,0,0)
    {}

//*********

    preorder_iterator(CKnot *kn, CTree *tr, CKnot *be)
      : _iterator(kn,tr,be)
    {}

//*********

    preorder_iterator(const preorder_iterator &i){
      *this=i;
    }

//*********

    preorder_iterator(const _iterator &i){
      *this=i;
    }

//*********

    preorder_iterator &operator++() {
/*
** Ende bereits erreicht?
** => Nichts machen
*/
      if(!m_knot)
        return(*this);

/*
** Linker Ast noch nicht abgearbeitet?
** => Ersten Knoten des linken Astes bestimmen
*/
      if(m_knot->m_left) {
        m_knot=m_knot->m_left;
      }

/*
** Linken Ast, aber rechten Ast noch nicht abgearbeitet?
** => Ersten Knoten des rechten Astes bestimmen
*/
      else if(m_knot->m_right) {
        m_knot=m_knot->m_right;
      }

/*
** Beide Äste abgearbeitet?
** => aufsteigen und nächsten Teilast finden
*/
      else {
        CKnot * son;

/*
** Laufe solange, wie Iterator im linken Ast aufsteigt und
** kein rechter Ast verfügbar ist oder der Iterator im rechten
** Ast aufsteigt
*/
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(((son==m_knot->m_left)&&(!m_knot->m_right) )||(son==m_knot->m_right)));
        if(m_knot)
          m_knot=m_knot->m_right;
      }
      return(*this);
    }

//*********

    preorder_iterator &operator--() {
/*
** Besitzt Iterator Endposition?
** => Letztes Element der Durchlauf-Reihenfolge bestimmen
*/
      if(!m_knot) {
        *this=m_tree->preorder_begin();
        m_knot=m_tree->m_root;
        if(m_knot) {
          while((m_knot->m_left)||(m_knot->m_right)) {
            if(m_knot->m_right)
              m_knot=m_knot->m_right;
            else
              m_knot=m_knot->m_left;
          }
        }
       return(*this);
      }

/*
** Besitzt Iterator Anfangsposition?
** => Nichts machen
*/
      if(m_knot==m_begin)
        return(*this);

/*
** Vater ermitteln
*/
      CKnot *son=m_knot;
      m_knot=m_knot->m_father;
      if(m_knot) {
/*
** Linker Ast noch nicht abgearbeitet?
** => Linken Ast so weit wie möglich hinabsteigen,
**    wobei rechte Teiläste Priorität haben
*/
        if((m_knot->m_left)&&((m_knot->m_left!=son))) {
          m_knot=m_knot->m_left;
          while((m_knot->m_left)||(m_knot->m_right)) {
            if(m_knot->m_right)
              m_knot=m_knot->m_right;
            else
              m_knot=m_knot->m_left;
          }
        }
      }
      return(*this);
    }

//*********

    preorder_iterator operator++(int) {
      preorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    preorder_iterator operator--(int) {
      preorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    preorder_iterator &operator=(const preorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    preorder_iterator &operator=(const _iterator &i) {
      *this=i.m_tree->preorder_begin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }
  };

//
//*********************************************************
// levelorder_iterator
//*********************************************************
//
  class levelorder_iterator;
  friend class levelorder_iterator;
  class levelorder_iterator : public std::iterator<std::forward_iterator_tag, value_type> {
  private:
    CKnot *m_knot;
    CTree *m_tree;
    int m_level;
    std::deque<CKnot*> m_queue;
  public:
    levelorder_iterator()
      :m_knot(0), m_tree(0),m_level(0) {
      m_queue.push_back(0);
    }

//*********

    int getLevel() const {
      return(m_level);
    }
//*********

    levelorder_iterator(CKnot *kn, CTree *tr)
      : m_knot(kn), m_tree(tr),m_level(0) {
      m_queue.push_back(0);
    }

//*********
    levelorder_iterator &operator++() {
/*
** Bevor nächster Knoten aus Queue geholt wird,
** die beiden Söhne (falls vorhanden), an die Queue
** anhängen
*/
      if(m_knot->m_left)
        m_queue.push_back(m_knot->m_left);
      if(m_knot->m_right)
        m_queue.push_back(m_knot->m_right);

/*
** Nächsten Knoten aus Queue holen
*/
      if(m_queue.size()!=0) {
        m_knot=m_queue.front();
        m_queue.pop_front();

/*
** Nächsten Level erreicht?
** => Neue Markierung in die Queue schieben und 
**    Level um eins erhöhen
*/
        if(!m_knot) {
          m_level++;
          m_queue.push_back(0);
          m_knot=m_queue.front();
          m_queue.pop_front();
        }
      }
      
/*
** Queue leer?
** => Ende des Baumes erreicht
*/
      else
        m_knot=0;
      return(*this);
    }

//*********

    levelorder_iterator operator++(int) {
      levelorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    reference operator*() {
      return(m_knot->m_data);
    }

//*********

    pointer operator->() {
      return(&(m_knot->m_data));
    }

//*********

    bool operator==(levelorder_iterator &i) const {
      return(m_knot==i.m_knot);
    }

//*********

    bool operator!=(levelorder_iterator &i) const {
      return(m_knot!=i.m_knot);
    }

  };

//
//*********************************************************
// reverse_inorder_iterator
//*********************************************************
//
  class reverse_inorder_iterator;
  friend class reverse_inorder_iterator;
  class reverse_inorder_iterator : public _iterator {
  public:
    reverse_inorder_iterator()
      : _iterator(0,0,0)
    {}

//*********

    reverse_inorder_iterator(CKnot *kn, CTree *tr, CKnot *be)
      : _iterator(kn,tr,be)
    {}

//*********

    reverse_inorder_iterator(const reverse_inorder_iterator &i){
      *this=i;
    }

//*********

    reverse_inorder_iterator(const _iterator &i){
      *this=i;
    }

//*********

    reverse_inorder_iterator &operator++() {
/*
** Ende bereits erreicht?
** => Nichts machen
*/
      if(!m_knot)
        return(*this);

/*
** Existiert linker Sohn?
** => Den rechten Ast des linken Sohns
**    komplett hinabsteigen
*/
      if(m_knot->m_left) {
        m_knot=m_knot->m_left;
        while(m_knot->m_right)
          m_knot=m_knot->m_right;
      }
/*
** Existiert kein linker Sohn?
** ==> So lange hinaufsteigen, bis aktueller Knoten
**     der rechte Sohn des Vaters ist.
**     Der Vater ist der neue Knoten
*/
      else {
        CKnot *son;
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(m_knot->m_left==son));
      }
      return(*this);
    }

//*********

    reverse_inorder_iterator &operator--() {
/*
** Besitz Iterator Endposition?
** => Letztes Element der Durchlauf-Reihenfolge bestimmen
*/
      if(!m_knot) {
        *this=m_tree->inorder_begin();
       return(*this);
      }

/*
** Besitzt Iterator Anfangsposition?
** => Nichts machen
*/
      if(m_knot==m_begin)
        return(*this);

/*
** Existiert rechter Sohn?
** => Den linken Ast des rechten Sohns
**    komplett hinabsteigen
*/
      if(m_knot->m_right) {
        m_knot=m_knot->m_right;
        while(m_knot->m_left)
          m_knot=m_knot->m_left;
      }

/*
** Existiert kein rechter Sohn?
** ==> So lange hinaufsteigen, bis aktueller Knoten
**     der linke Sohn des Vaters ist.
**     Der Vater ist der neue Knoten
*/
      else {
        CKnot *son;
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(m_knot->m_right==son));
      }
      return(*this);
    }

//*********

    reverse_inorder_iterator operator++(int) {
      reverse_inorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    reverse_inorder_iterator operator--(int) {
      reverse_inorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    reverse_inorder_iterator &operator=(const reverse_inorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    reverse_inorder_iterator &operator=(const _iterator &i) {
      *this=i.m_tree->inorder_rbegin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }

  };

//
//*********************************************************
// reverse_postorder_iterator
//*********************************************************
//
  class reverse_postorder_iterator;
  friend class reverse_postorder_iterator;
  class reverse_postorder_iterator : public _iterator {
  public:
    reverse_postorder_iterator()
      : _iterator(0,0,0)
    {}

//*********

    reverse_postorder_iterator(CKnot *kn, CTree *tr, CKnot *be)
      : _iterator(kn,tr,be)
    {}

//*********

    reverse_postorder_iterator(const reverse_postorder_iterator &i){
      *this=i;
    }

//*********

    reverse_postorder_iterator(const _iterator &i){
      *this=i;
    }

//*********

    reverse_postorder_iterator &operator++() {
/*
** Ende bereits erreicht?
** => Nichts machen
*/
      if(!m_knot)
        return(*this);

/*
** Rechter Ast noch nicht abgearbeitet?
** => Ersten Knoten des rechten Astes bestimmen
*/
      if(m_knot->m_right) {
        m_knot=m_knot->m_right;
      }

/*
** Rechten Ast, aber linken Ast noch nicht abgearbeitet?
** => Ersten Knoten des linken Astes bestimmen
*/
      else if(m_knot->m_left) {
        m_knot=m_knot->m_left;
      }

/*
** Beide Äste abgearbeitet?
** => aufsteigen und nächsten Teilast finden
*/
      else {
        CKnot * son;
/*
** Laufe solange, wie Iterator im rechten Ast aufsteigt und
** kein linker Ast verfügbar ist oder der Iterator im linken
** Ast aufsteigt
*/
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(((son==m_knot->m_right)&&(!m_knot->m_left) )||(son==m_knot->m_left)));
        if(m_knot)
          m_knot=m_knot->m_left;
      }

      return(*this);
    }

//*********

    reverse_postorder_iterator &operator--() {
/*
** Besitzt Iterator Endposition?
** => Letztes Element der Durchlauf-Reihenfolge bestimmen
*/
      if(!m_knot) {
        *this=m_tree->postorder_begin();
       return(*this);
      }

/*
** Besitzt Iterator Anfangsposition?
** => Nichts machen
*/
      if(m_knot==m_begin)
        return(*this);

/*
** Vater ermitteln
*/
      CKnot *son=m_knot;
      m_knot=m_knot->m_father;
      if(m_knot) {

/*
** Rechter Ast noch nicht abgearbeitet?
** => Rechten Ast so weit wie möglich hinabsteigen,
**    wobei linke Teiläste Priorität haben
*/
        if((m_knot->m_right)&&((m_knot->m_right!=son))) {
          m_knot=m_knot->m_right;
          while((m_knot->m_left)||(m_knot->m_right)) {
            if(m_knot->m_left)
              m_knot=m_knot->m_left;
            else
              m_knot=m_knot->m_right;
          }
        }
      }

      return(*this);
    }

//*********

    reverse_postorder_iterator operator++(int) {
      reverse_postorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    reverse_postorder_iterator operator--(int) {
      reverse_postorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    reverse_postorder_iterator &operator=(const reverse_postorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    reverse_postorder_iterator &operator=(const _iterator &i) {
      *this=i.m_tree->postorder_rbegin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }

  };

//
//*********************************************************
// reverse_preorder_iterator
//*********************************************************
//
  class reverse_preorder_iterator;
  friend class reverse_preorder_iterator;
  class reverse_preorder_iterator : public _iterator {
  public:
    reverse_preorder_iterator()
      : _iterator(0,0,0)
    {}

//*********

    reverse_preorder_iterator(CKnot *kn, CTree *tr, CKnot *be)
      : iterator(kn,tr,be)
    {}

//*********

    reverse_preorder_iterator(const reverse_preorder_iterator &i){
      *this=i;
    }

//*********

    reverse_preorder_iterator(const _iterator &i){
      *this=i;
    }

//*********

    reverse_preorder_iterator &operator++() {
/*
** Ende bereits erreicht?
** => Nichts machen
*/
      if(!m_knot)
        return(*this);

/*
** Vater ermitteln
*/
      CKnot *son=m_knot;
      m_knot=m_knot->m_father;
      if(m_knot) {

/*
** Linker Ast noch nicht abgearbeitet?
** => Linken Ast so weit wie möglich hinabsteigen,
**    wobei rechte Teiläste Priorität haben
*/
        if((m_knot->m_left)&&((m_knot->m_left!=son))) {
          m_knot=m_knot->m_left;
          while((m_knot->m_left)||(m_knot->m_right)) {
            if(m_knot->m_right)
              m_knot=m_knot->m_right;
            else
              m_knot=m_knot->m_left;
          }
        }
      }
      return(*this);
    }

//*********

    reverse_preorder_iterator &operator--() {
/*
** Besitzt Iterator Endposition?
** => Letztes Element der Durchlauf-Reihenfolge bestimmen
*/
      if(!m_knot) {
        *this=m_tree->preorder_begin();
       return(*this);
      }

/*
** Besitzt Iterator Anfangsposition?
** => Nichts machen
*/
      if(m_knot==m_begin)
        return(*this);

/*
** Linker Ast noch nicht abgearbeitet?
** => Ersten Knoten des linken Astes bestimmen
*/
      if(m_knot->m_left) {
        m_knot=m_knot->m_left;
      }

/*
** Linken Ast, aber rechten Ast noch nicht abgearbeitet?
** => Ersten Knoten des rechten Astes bestimmen
*/
      else if(m_knot->m_right) {
        m_knot=m_knot->m_right;
      }

/*
** Beide Äste abgearbeitet?
** => aufsteigen und nächsten Teilast finden
*/
      else {
        CKnot * son;

/*
** Laufe solange, wie Iterator im linken Ast aufsteigt und
** kein rechter Ast verfügbar ist oder der Iterator im rechten
** Ast aufsteigt
*/
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(((son==m_knot->m_left)&&(!m_knot->m_right) )||(son==m_knot->m_right)));
        if(m_knot)
          m_knot=m_knot->m_right;
      }
      return(*this);
    }

//*********

    reverse_preorder_iterator operator++(int) {
      reverse_preorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    reverse_preorder_iterator operator--(int) {
      reverse_preorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    reverse_preorder_iterator &operator=(const reverse_preorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    reverse_preorder_iterator &operator=(const _iterator &i) {
      *this=i.m_tree->preorder_rbegin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }
  };

//
//*********************************************************
// const_inorder_iterator
//*********************************************************
//
  class const_inorder_iterator;
  friend class const_inorder_iterator;
  class const_inorder_iterator : public _const_iterator {
  public:
    const_inorder_iterator()
      : _const_iterator(0,0,0)
    {}

//*********

    const_inorder_iterator(CKnot *kn, const CTree *tr, CKnot *be)
      : _const_iterator(kn,tr,be)
    {}

//*********

    const_inorder_iterator(const const_inorder_iterator &i){
      *this=i;
    }

//*********

    const_inorder_iterator(const _const_iterator &i){
      *this=i;
    }

//*********

    const_inorder_iterator &operator++() {
      if(!m_knot)
        return(*this);

      if(m_knot->m_right) {
        m_knot=m_knot->m_right;
        while(m_knot->m_left)
          m_knot=m_knot->m_left;
      }
      else {
        CKnot *son;
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(m_knot->m_right==son));
      }
      return(*this);
    }

//*********

    const_inorder_iterator &operator--() {
      if(!m_knot) {
        *this=m_tree->inorder_begin();
        m_knot=m_tree->m_root;
        if(m_knot) {
          while(m_knot->m_right)
            m_knot=m_knot->m_right;
        }
       return(*this);
      }
      if(m_knot==m_begin)
        return(*this);

      if(m_knot->m_left) {
        m_knot=m_knot->m_left;
        while(m_knot->m_right)
          m_knot=m_knot->m_right;
      }
      else {
        CKnot *son;
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(m_knot->m_left==son));
      }
      return(*this);
    }

//*********

    const_inorder_iterator operator++(int) {
      const_inorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    const_inorder_iterator operator--(int) {
      const_inorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    const_inorder_iterator &operator=(const const_inorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    const_inorder_iterator &operator=(const _const_iterator &i) {
      *this=i.m_tree->inorder_begin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }

  };

//
//*********************************************************
// const_postorder_iterator
//*********************************************************
//
  class const_postorder_iterator;
  friend class const_postorder_iterator;
  class const_postorder_iterator : public _const_iterator {
  public:
    const_postorder_iterator()
      : _const_iterator(0,0,0)
    {}

//*********

    const_postorder_iterator(CKnot *kn, const CTree *tr, CKnot *be)
      : _const_iterator(kn,tr,be)
    {}

//*********

    const_postorder_iterator(const const_postorder_iterator &i){
      *this=i;
    }

//*********

    const_postorder_iterator(const _const_iterator &i){
      *this=i;
    }

//*********

    const_postorder_iterator &operator++() {
      if(!m_knot)
        return(*this);

      CKnot *son=m_knot;
      m_knot=m_knot->m_father;
      if(m_knot) {
        if((m_knot->m_right)&&((m_knot->m_right!=son))) {
          m_knot=m_knot->m_right;
          while((m_knot->m_left)||(m_knot->m_right)) {
            if(m_knot->m_left)
              m_knot=m_knot->m_left;
            else
              m_knot=m_knot->m_right;
          }
        }
      }

      return(*this);
    }

//*********

    const_postorder_iterator &operator--() {
      if(!m_knot) {
        *this=m_tree->postorder_begin();
        m_knot=m_tree->m_root;
       return(*this);
      }

      if(m_knot==m_begin)
        return(*this);

      if(m_knot->m_right) {
        m_knot=m_knot->m_right;
      }
      else if(m_knot->m_left) {
        m_knot=m_knot->m_left;
      }
      else {
        CKnot * son;
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(((son==m_knot->m_right)&&(!m_knot->m_left) )||(son==m_knot->m_left)));
        if(m_knot)
          m_knot=m_knot->m_left;
      }

      return(*this);
    }

//*********

    const_postorder_iterator operator++(int) {
      const_postorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    const_postorder_iterator operator--(int) {
      const_postorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    const_postorder_iterator &operator=(const const_postorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    const_postorder_iterator &operator=(const _const_iterator &i) {
      *this=i.m_tree->postorder_begin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }

  };

//
//*********************************************************
// const_preorder_iterator
//*********************************************************
//
  class const_preorder_iterator;
  friend class const_preorder_iterator;
  class const_preorder_iterator : public _const_iterator {
  public:
    const_preorder_iterator()
      : _const_iterator(0,0,0)
    {}

//*********

    const_preorder_iterator(CKnot *kn, const CTree *tr, CKnot *be)
      : _const_iterator(kn,tr,be)
    {}

//*********

    const_preorder_iterator(const const_preorder_iterator &i){
      *this=i;
    }

//*********

    const_preorder_iterator(const _const_iterator &i){
      *this=i;
    }

//*********

    const_preorder_iterator &operator++() {
      if(!m_knot)
        return(*this);

      if(m_knot->m_left) {
        m_knot=m_knot->m_left;
      }
      else if(m_knot->m_right) {
        m_knot=m_knot->m_right;
      }
      else {
        CKnot * son;
        do {
          son=m_knot;
          m_knot=m_knot->m_father;
        } while((m_knot)&&(((son==m_knot->m_left)&&(!m_knot->m_right) )||(son==m_knot->m_right)));
        if(m_knot)
          m_knot=m_knot->m_right;
      }
      return(*this);
    }

//*********

    const_preorder_iterator &operator--() {
      if(!m_knot) {
        *this=m_tree->preorder_begin();
        m_knot=m_tree->m_root;
        if(m_knot) {
          while((m_knot->m_left)||(m_knot->m_right)) {
            if(m_knot->m_right)
              m_knot=m_knot->m_right;
            else
              m_knot=m_knot->m_left;
          }
        }
       return(*this);
      }

      if(m_knot==m_begin)
        return(*this);

      CKnot *son=m_knot;
      m_knot=m_knot->m_father;
      if(m_knot) {
        if((m_knot->m_left)&&((m_knot->m_left!=son))) {
          m_knot=m_knot->m_left;
          while((m_knot->m_left)||(m_knot->m_right)) {
            if(m_knot->m_right)
              m_knot=m_knot->m_right;
            else
              m_knot=m_knot->m_left;
          }
        }
      }
      return(*this);
    }

//*********

    const_preorder_iterator operator++(int) {
      const_preorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    const_preorder_iterator operator--(int) {
      const_preorder_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    const_preorder_iterator &operator=(const const_preorder_iterator &i) {
      m_knot=i.m_knot;
      m_begin=i.m_begin;
      m_tree=i.m_tree;
      return(*this);
    }

//*********

    const_preorder_iterator &operator=(const _const_iterator &i) {
      *this=i.m_tree->const_preorder_begin();
      m_knot=i.m_knot;
      m_tree=i.m_tree;
      return(*this);
    }
  };

//
//*********************************************************
// const_levelorder_iterator
//*********************************************************
//
  class const_levelorder_iterator;
  friend class const_levelorder_iterator;
  class const_levelorder_iterator : public std::iterator<std::forward_iterator_tag, value_type> {
  private:
    CKnot *m_knot;
    const CTree *m_tree;
    int m_level;
    std::deque<CKnot*> m_queue;
  public:
    const_levelorder_iterator()
      :m_knot(0), m_tree(0),m_level(0) {
      m_queue.push_back(0);
    }

//*********

    int getLevel() const {
      return(m_level);
    }
//*********

    const_levelorder_iterator(CKnot *kn, const CTree *tr)
      : m_knot(kn), m_tree(tr),m_level(0) {
      m_queue.push_back(0);
    }

//*********
    const_levelorder_iterator &operator++() {
      if(m_knot->m_left)
        m_queue.push_back(m_knot->m_left);
      if(m_knot->m_right)
        m_queue.push_back(m_knot->m_right);
      if(m_queue.size()!=0) {
        m_knot=m_queue.front();
        m_queue.pop_front();
        if(!m_knot) {
          m_level++;
          m_queue.push_back(0);
          m_knot=m_queue.front();
          m_queue.pop_front();
        }
      }
      else
        m_knot=0;
      return(*this);
    }

//*********

    const_levelorder_iterator operator++(int) {
      const_levelorder_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    const_reference operator*() {
      return(m_knot->m_data);
    }

//*********

    const_pointer operator->() {
      return(&(m_knot->m_data));
    }

//*********

    bool operator==(const_levelorder_iterator &i) const {
      return(m_knot==i.m_knot);
    }

//*********

    bool operator!=(const_levelorder_iterator &i) const {
      return(m_knot!=i.m_knot);
    }

  };

//
//*********************************************************
// begin und end-Methoden
//*********************************************************
//

  typedef inorder_iterator iterator;
  typedef const_inorder_iterator const_iterator;

  inorder_iterator inorder_begin() {
    CKnot *kn=m_root;
    if(kn) {
      while(kn->m_left)
        kn=kn->m_left;
    }
    return(inorder_iterator(kn,this,kn));
  }

  inorder_iterator inorder_end() {
    return(inorder_iterator(0,this,0));
  }

  reverse_inorder_iterator inorder_rbegin() {
    CKnot *kn=m_root;
    if(kn) {
      while(kn->m_right)
        kn=kn->m_right;
    }
    return(reverse_inorder_iterator(kn,this,kn));
  }

  reverse_inorder_iterator inorder_rend() {
    return(reverse_inorder_iterator(0,this,0));
  }

  postorder_iterator postorder_begin() {
    CKnot *kn=m_root;
    if(kn) {
      while((kn->m_left)||(kn->m_right)) {
        if(kn->m_left)
          kn=kn->m_left;
        else
          kn=kn->m_right;
      }
    }
    return(postorder_iterator(kn,this,kn));
  }

  postorder_iterator postorder_end() {
    return(postorder_iterator(0,this,0));
  }

  reverse_postorder_iterator postorder_rbegin() {
    return(reverse_postorder_iterator(m_root,this,m_root));
  }

  reverse_postorder_iterator postorder_rend() {
    return(reverse_postorder_iterator(0,this,0));
  }

  preorder_iterator preorder_begin() {
    return(preorder_iterator(m_root,this, m_root));
  }

  preorder_iterator preorder_end() {
    return(preorder_iterator(0,this,0));
  }

  reverse_preorder_iterator preorder_rbegin() {
    CKnot *kn=m_root;
    if(kn) {
      while(kn->m_right)
        kn=kn->m_right;
    }

    return(reverse_preorder_iterator(kn,this, kn));
  }

  reverse_preorder_iterator preorder_rend() {
    return(reverse_preorder_iterator(0,this,0));
  }

  levelorder_iterator levelorder_begin() {
    return(levelorder_iterator(m_root,this));
  }

  levelorder_iterator levelorder_end() {
    return(levelorder_iterator(0,this));
  }



  const_inorder_iterator inorder_begin() const {
    CKnot *kn=m_root;
    if(kn) {
      while(kn->m_left)
        kn=kn->m_left;
    }
    return(const_inorder_iterator(kn,this,kn));
  }

  const_inorder_iterator inorder_end() const {
    return(const_inorder_iterator(0,this,0));
  }
  const_postorder_iterator postorder_begin() const {
    CKnot *kn=m_root;
    if(kn) {
      while((kn->m_left)||(kn->m_right)) {
        if(kn->m_left)
          kn=kn->m_left;
        else
          kn=kn->m_right;
      }
    }
    return(const_postorder_iterator(kn,this,kn));
  }

  const_postorder_iterator postorder_end() const {
    return(const_postorder_iterator(0,this,0));
  }


  const_preorder_iterator preorder_begin() const {
    return(const_preorder_iterator(m_root,this, m_root));
  }

  const_preorder_iterator preorder_end() const {
    return(const_preorder_iterator(0,this,0));
  }
  const_levelorder_iterator levelorder_begin() const {
    return(const_levelorder_iterator(m_root,this));
  }

  const_levelorder_iterator levelorder_end() const {
    return(const_levelorder_iterator(0,this));
  }




//*********
  CTree()
  : m_root(0), m_size(0)
  {}

//*********
  CTree(const CTree &t)
  : m_root(0), m_size(0) {
    *this=t;
  }

//*********
  template<typename Input>
  CTree(Input beg, Input end)
  : m_root(0), m_size(0) {
    insert(beg, end);
  }

//*********
  virtual ~CTree() {
    if(m_root)
      deleteKnot(m_root);
  }



//*********
  virtual inorder_iterator insert(const value_type &v) {
/*
** Datenobjekt in einen Knoten packen
*/
    CKnot *kn=new CKnot(0,0,0,v);
/*
** Knoten in Baum einfügen
*/
    insert(kn);
/*
** Position als Iterator zurück geben
*/
    return(_iterator(kn,this,0));
  }

//*********
  template<typename Input>
  void insert(Input beg, Input end) {
    while(beg!=end)
      insert(*(beg++));
  }

//*********
  inorder_iterator insert(_iterator i, const value_type &v) {
    return(insert(v));
  }
//*********
  inorder_iterator find(const key_type &k) {
    return(_iterator(findFirstKnot(k),this,0));
  }


//*********
  const_inorder_iterator find(const key_type &k) const {
    return(_const_iterator(findFirstKnot(k),this,0));
  }
//*********
  inorder_iterator lower_bound(const key_type &k) {
    return(_iterator(findFirstKnot(k),this,0));
  }
//*********
  const_inorder_iterator lower_bound(const key_type &k) const{
    return(_const_iterator(findFirstKnot(k),this,0));
  }

//*********
  inorder_iterator upper_bound(const key_type &k) {
    inorder_iterator i=_iterator(findLastKnot(k),this,0);
    ++i;
    return(i);
  }
//*********
  const_inorder_iterator upper_bound(const key_type &k) const {
    const_inorder_iterator i=_const_iterator(findLastKnot(k),this,0);
    ++i;
    return(i);
  }

//*********
  virtual size_type erase(const key_type &k) {
/*
** Ersten Knoten mit Schlüssel k finden
*/
    CKnot *kn=findFirstKnot(k);
/*
** Keine Knoten vorhanden?
** => Kein Knoten gelöscht
*/
    if(!kn)
      return(0);
/*
** Inorder-Iterator auf Knoten hinter dem zu löschenden
** Knoten setzen
*/
    inorder_iterator i=inorder_iterator(kn,this,0);
    ++i;
    size_type a=0;
/*
** So lange laufen, wie noch Knoten mit 
** Schlüssel k existieren
*/
    do {
/*
** Schlüssel löschen, neuen Knoten aus Iterator
** holen und Iterator inkrementieren
*/
      erase(kn);
      a++;
      kn=i.m_knot;
      ++i;
    } while((kn)&&(Traits::eq(Traits::getKey(kn->m_data),k)));
    return(a);
  }
//*********
  virtual inorder_iterator erase(_iterator i) {
/*
** Iterator-Position hinter zu löschendem
** Knoten ermitteln
** (Ohne Anfangs-Position für Iterator zu bestimmen)
*/
    inorder_iterator io(i.m_knot,this,0);
    ++io;
/*
** konnte Knoten gelöscht werden?
** => Position hinter geköschtem Knoten zurückgeben
** Andernfalls End-Position zurückgeben
*/
    if(erase(i.m_knot))
      return(_iterator(io.m_knot,this,0));
    else
      return(inorder_end());
  }

//*********
  virtual inorder_iterator erase(inorder_iterator beg, inorder_iterator end) {
    while((beg!=inorder_end())&&(beg!=end)) {
      CKnot *kn=beg.m_knot;
      ++beg;
      erase(kn);
    }
    return(beg);
  }

//*********
  size_type size() const  {
    return(m_size);
  }
//*********
  bool empty() const {
    return(m_size==0);
  }
//*********
  value_type &front() {
    return(inorder_begin().m_knot->m_data);
  }
//*********
  const value_type &front() const {
    return(inorder_begin().m_knot->m_data);
  }
//*********
  value_type &back() {
    return(inorder_rbegin().m_knot->m_data);
  }
//*********
  const value_type &back() const{
    return(inorder_rbegin().m_knot->m_data);
  }
//*********
  virtual void pop_front() {
    if(!empty())
      erase(inorder_begin().m_knot);
  }
//*********
  virtual void pop_back() {
    if(!empty())
      erase(inorder_rbegin().m_knot);
  }


//*********
  virtual void push_front(const value_type &v) {
    insert(v);
  }
//*********
  virtual void push_back(const value_type &v) {
    insert(v);
  }
//*********
  CTree &operator=(const CTree &t) {
    if(&t!=this) {
/*
** Alle Knoten des Baums löschen
*/
      deleteKnot(m_root);
      m_size=t.m_size;
/*
** Besitzt Quellbaum Knoten?
** = Wurzelknoten kopieren und alle weitern Knoten
** durch Aufruf von copySons kopieren
*/
      if(t.m_root) {
        m_root=new CKnot(0,0,0,t.m_root->m_data);
        copySons(m_root, t.m_root);
      }
    }
    return(*this);
  }
//*********
  bool checkConsistency(CKnot *kn) {
    if(kn->m_left) {
      if(kn->m_left->m_father!=kn)
        return(false);
      if(!checkConsistency(kn->m_left))
        return(false);
    }
    if(kn->m_right) {
      if(kn->m_right->m_father!=kn)
        return(false);
      if(!checkConsistency(kn->m_right))
        return(false);
    }
    return(true);
  }
  bool checkConsistency() {
    if(!m_root)
      return(true);
    if(m_root->m_father)
      return(false);
    return(checkConsistency(m_root));
  }
};


}; // namespace Codebook

#endif
