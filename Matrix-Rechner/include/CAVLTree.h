
#ifndef __CAVLTREE_H
#define __CAVLTREE_H

#include "CTree.h"


namespace Codebook {

template<typename Traits>
class CAVLTree : public CTree<Traits> {
public:
  typedef CTree<Traits> base_type;
  typedef CAVLTree<Traits> my_type;
  typedef typename Traits::value_type value_type;
  typedef typename Traits::key_type key_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef short balance_type;


//*********************************************************
// CAVLKnot
//*********************************************************
  class CAVLKnot : public base_type::CKnot {
  public:
    balance_type m_balance;

    CAVLKnot(CAVLKnot *fa, CAVLKnot *li, CAVLKnot *re)
    : CKnot(fa,li,re),m_balance(0)
    { }

//*********
    CAVLKnot(CAVLKnot *fa, CAVLKnot *li, CAVLKnot *re, const value_type &v)
    : CKnot(fa,li,re,v),m_balance(0)
    {}

//*********
    CAVLKnot(CAVLKnot *fa, CAVLKnot *li, CAVLKnot *re, const value_type &v, balance_type b)
    : CKnot(fa,li,re,v),m_balance(b)
    {}

//*********
    virtual ~CAVLKnot()
    {}

//*********
    CAVLKnot *getLeft() const {
      return(reinterpret_cast<CAVLKnot*>(m_left));
    }
//*********
    CAVLKnot *getRight() const {
      return(reinterpret_cast<CAVLKnot*>(m_right));
    }
//*********
    CAVLKnot *getFather() const {
      return(reinterpret_cast<CAVLKnot*>(m_father));
    }
  };

private:
  void copySons(CAVLKnot* d, CAVLKnot *s) {
/*
** Beide Knoten existent?
*/
    if(d&&s) {

/*
** Linken Sohn kopieren, falls vorhanden und
** f�r ihn copySons rekursiv aufrufen
*/
      if(s->m_left) {
        d->m_left=new CAVLKnot(d,0,0,s->m_left->m_data,reinterpret_cast<CAVLKnot*>(s->m_left)->m_balance);
        copySons(reinterpret_cast<CAVLKnot*>(d->m_left),reinterpret_cast<CAVLKnot*>(s->m_left));
      }

/*
** Rechten Sohn kopieren, falls vorhanden und
** f�r ihn copySons rekursiv aufrufen
*/
      if(s->m_right) {
        d->m_right=new CAVLKnot(d,0,0,s->m_right->m_data,reinterpret_cast<CAVLKnot*>(s->m_right)->m_balance);
        copySons(reinterpret_cast<CAVLKnot*>(d->m_right),reinterpret_cast<CAVLKnot*>(s->m_right));
      }
    }
  }
//*********
  CAVLKnot *rotate(CAVLKnot *kn) {
    CAVLKnot *child;

    if(kn->m_balance<0) {
      child=kn->getLeft();
      kn->m_left=child->m_right;
      if(child->m_right)
        child->m_right->m_father=kn;
      child->m_right=kn;
      child->m_father=kn->m_father;
      kn->m_father=child;
      if(child->m_father) {
        if(child->m_father->m_left==kn)
          child->m_father->m_left=child;
        else
          child->m_father->m_right=child;
      }
      else
        m_root=child;

      if(kn->m_balance==-1) {
        if(child->m_balance==1) {
          child->m_balance=2;
          kn->m_balance=0;
          return(child);
        }

        if(child->m_balance==-1)
          kn->m_balance=1;
        else
          kn->m_balance=0;
        child->m_balance=1;
        return(child);
      }
      if(kn->m_balance==-2) {
        if(child->m_balance==-1) {
          kn->m_balance=child->m_balance=0;
          return(child);
        }
        if(child->m_balance==0) {
          kn->m_balance=-1;
          child->m_balance=1;
          return(child);
        }
        if(child->m_balance==-2) {
          kn->m_balance=1;
          child->m_balance=0;
          return(child);
        }
      }
    }
    else {
      child=kn->getRight();
      kn->m_right=child->m_left;
      if(child->m_left) child->m_left->m_father=kn;
      child->m_left=kn;
      child->m_father=kn->m_father;
      kn->m_father=child;
      if(child->m_father) {
        if(child->m_father->m_left==kn)
          child->m_father->m_left=child;
        else
          child->m_father->m_right=child;
      }
      else
        m_root=child;

      if(kn->m_balance==1) {
        if(child->m_balance==-1) {
//            Inorder();
          child->m_balance=-2;
          kn->m_balance=0;
          return(child);;
        }

        if(child->m_balance==1)
          kn->m_balance=-1;
        else
          kn->m_balance=0;
        child->m_balance=-1;
        return(child);
      }
      if(kn->m_balance==2) {
        if(child->m_balance==1)  {
          kn->m_balance=child->m_balance=0;
          return(child);
        }
        if(child->m_balance==0)  {
          kn->m_balance=1;
          child->m_balance=-1;
          return(child);
        }
        if(child->m_balance==2)  {
          kn->m_balance=-1;
          child->m_balance=0;
          return(child);
        }
      }
    }
    return(child);
  }

//*********
  void rebalanceInsert(CAVLKnot *kn) {
    CAVLKnot *fkn=kn->getFather();

    if(((kn->m_balance==-1)||(kn->m_balance==1))&&(kn!=m_root)) {
      if(kn->m_father->m_left==kn)
        kn->getFather()->m_balance--;
      else
        kn->getFather()->m_balance++;
      rebalanceInsert(kn->getFather());
      return;
    }

    if(kn->m_balance==-2) {
      if(kn->getLeft()->m_balance==-1) {
        rotate(kn);
        return;
      }
      else {
        rotate(kn->getLeft());
        rotate(kn);
        return;
      }
    }

     if(kn->m_balance==2) {
      if(kn->getRight()->m_balance==1) {
        rotate(kn);
        return;
      }
      else {
        rotate(kn->getRight());
        rotate(kn);
        return;
      }
    }
  }


//*********
  void rebalanceErase(CAVLKnot *kn) {
    CAVLKnot *fkn=kn->getFather();

    if((kn->m_balance==-1)||(kn->m_balance==1))
      return;
    if((kn==m_root)&&(kn->m_balance==0))
      return;

    if(kn==m_root) {
      if(kn->m_balance==-2) {
        if(kn->getLeft()->m_balance<=0)
          rotate(kn);
        else {
          kn=rotate(kn->getLeft());
          rotate(kn);
        }
      }
      else {
        if(kn->getRight()->m_balance>=0)
          rotate(kn);
        else {
          kn=rotate(kn->getRight());
          rotate(kn);
        }
      }
      return;
    }
    if(kn->m_balance==2) {
      switch(kn->getRight()->m_balance) {
        case 0:
          rotate(kn);
          return;

        case 1:
          rebalanceErase(rotate(kn));
          return;

        case -1:
          rotate(kn->getRight());
          rebalanceErase(rotate(kn));
          return;
      }
    }
    if(kn->m_balance==-2) {
      switch(kn->getLeft()->m_balance) {
        case 0:
          rotate(kn);
          return;

        case -1:
          rebalanceErase(rotate(kn));
          return;

        case 1:
          rotate(kn->getLeft());
          rebalanceErase(rotate(kn));
          return;
      }
    }
    if(fkn->m_left==kn) {
      fkn->m_balance++;
      if(fkn->m_balance<2) {
        rebalanceErase(fkn);
        return;
      }
      switch(fkn->getRight()->m_balance) {
        case 0:
          rotate(fkn);
          return;

        case 1:
          rebalanceErase(rotate(fkn));
          return;

        case -1:
          rotate(fkn->getRight());
          rebalanceErase(rotate(fkn));
          return;
      }
    }
    if(fkn->m_right==kn) {
      fkn->m_balance--;
      if(fkn->m_balance>-2) {
        rebalanceErase(fkn);
        return;
      }
      switch(fkn->getLeft()->m_balance) {
        case 0:
          rotate(fkn);
          return;

        case -1:
          rebalanceErase(rotate(fkn));
          return;

        case 1:
          rotate(fkn->getLeft());
          rebalanceErase(rotate(fkn));
          return;
      }
    }
  }

//*********
  bool erase(CAVLKnot *cur) {
    if(!cur) return(false);

    CAVLKnot *father=cur->getFather();

/*
** Zu l�schender Knoten hat keine S�hne?
** => Kann problemlos gel�scht werden
*/
    if((!cur->m_left)&&(!cur->m_right)) {

/*
** Zu l�schender Knoten ist die Wurzel?
** => Baum leer
*/
      if(cur==m_root) {
        m_root=0;
        delete(cur);
        m_size--;
        return(false);
      }

/*
** Zu l�schender Knoten ist nicht die Wurzel?
** => Vater muss ber�cksichtigt werden
*/
      else {

/*
** Je nachdem, ob zu l�schender Knoten der linke oder
** rechte Sohn des Vaters ist, muss entsprechender Sohn
** des Vaters auf 0 gesetzt werden
*/
        if(father->m_left==cur) {
          father->m_left=0;
          father->m_balance++;
        }
        else {
          father->m_right=0;
          father->m_balance--;
        }

/*
** Knoten l�schen, AVL-Bedingung testen und
** ggfs. wieder herstellen
*/
        delete(cur);
        m_size--;
        rebalanceErase(father);
        return(true);
      }
    }

/*
** Besitzt zu l�schender Knoten zwei S�hne?
** => Zu l�schenden Knoten durch symmetrischen
**    Vorg�nger ersetzen und symmetrischen Vorg�nger
**    l�schen
*/
    if((cur->m_left)&&(cur->m_right)) {
      CAVLKnot *sys=reinterpret_cast<CAVLKnot*>(symmetricPred(cur));
      cur->m_data=sys->m_data;
      return(erase(sys));
    }

/*
** Besitzt zu l�schender Knoten nur einen Sohn?
** => Sohn des zu l�schenden Knotens wird Sohn vom
** Vater des zu l�schenden Knotens
*/
    CAVLKnot *son;
    if(cur->m_left)
      son=cur->getLeft();
    else
      son=cur->getRight();

    if(cur!=m_root) {
      son->m_father=father;
      if(father->m_left==cur) {
        father->m_left=son;
        father->m_balance++;
      }
      else {
        father->m_right=son;
        father->m_balance--;
      }

/*
** AVL-Bedingung testen und ggfs. wieder herstellen
*/
      rebalanceErase(father);
      }

/*
** Ist zu l�schender Sohn die Wurzel?
** => Sohn des zu l�schenden Knotens wird Wurzel
*/
    else {
      son->m_father=0;
      m_root=son;
    }

/*
** Knoten l�schen
*/
    delete(cur);
    m_size--;
    return(true);
  }


//*********
  void insert(CAVLKnot *kn) {
/*
** Baum leer?
** => Einzuf�gender Knoten wird die Wurzel
*/
    if(!m_root) {
      m_root=kn;
      m_size++;
      return;
    }

    CAVLKnot *cur=reinterpret_cast<CAVLKnot*>(m_root);
    while(cur) {
      if(Traits::gt(Traits::getKey(cur->m_data),Traits::getKey(kn->m_data))) {

/*
** Einzuf�gender Knoten kleiner als aktueller Knoten?
** => Falls vorhanden, im linken Teilbaum nach Einf�geposition
**    suchen. Andernfalls wird einzuf�gender Knoten linker
**    Sohn des aktuellen Knotens
*/
        if(!cur->m_left) {
          cur->m_left=kn;
          cur->m_balance--;
          kn->m_father=cur;
          m_size++;

/*
** Hat sich Balance von 0 auf anderen Wert ge�ndert
** => H�he hat sich ge�ndert, eventuell umstrukturieren
*/
          if(cur->m_balance)
            rebalanceInsert(cur);
          return;
        }
        else {
          cur=cur->getLeft();
        }
      }
      else {

/*
** Einzuf�gender Knoten gr��er/gleich dem aktuellen Knoten?
** => Falls vorhanden, im rechten Teilbaum nach Einf�geposition
**    suchen. Andernfalls wird einzuf�gender Knoten rechter
**    Sohn des aktuellen Knotens
*/
        if(!cur->m_right) {
          cur->m_right=kn;
          cur->m_balance++;
          kn->m_father=cur;
          m_size++;

/*
** Hat sich Balance von 0 auf anderen Wert ge�ndert
** => H�he hat sich ge�ndert, eventuell umstrukturieren
*/
          if(cur->m_balance)
            rebalanceInsert(cur);
          return;
        }
        else {
          cur=cur->getRight();
        }
      }
    }
  }

public:

  CAVLTree(void)
    : base_type() {
//      m_root=0;
//      m_size=0;
  }

//*********
  CAVLTree(const CAVLTree &t)
  : base_type() {
    *this=t;
  }

//*********
  template<typename Input>
  CAVLTree(Input beg, Input end)
  : base_type() {
    insert(beg, end);
  }
//*********
virtual ~CAVLTree()
{}



//*********
  virtual inorder_iterator insert(const value_type &v) {
/*
** Datenobjekt in einen Knoten packen
*/
    CAVLKnot *kn=new CAVLKnot(0,0,0,v);
/*
** Knoten in Baum einf�gen
*/
    insert(kn);
/*
** Position als Iterator zur�ck geben
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
  virtual size_type erase(const key_type &k) {
/*
** Ersten Knoten mit Schl�ssel k finden
*/
    CAVLKnot *kn=reinterpret_cast<CAVLKnot*>(findFirstKnot(k));
/*
** Keine Knoten vorhanden?
** => Kein Knoten gel�scht
*/
    if(!kn)
      return(0);
/*
** Inorder-Iterator auf Knoten hinter dem zu l�schenden
** Knoten setzen
*/
    inorder_iterator i=_iterator(kn,this,0);
    ++i;
    size_type a=0;
/*
** So lange laufen, wie noch Knoten mit 
** Schl�ssel k existieren
*/
    do {
/*
** Schl�ssel l�schen, neuen Knoten aus Iterator
** holen und Iterator inkrementieren
*/
      erase(kn);
      a++;
      kn=reinterpret_cast<CAVLKnot*>(i.m_knot);
      ++i;
    } while((kn)&&(Traits::eq(Traits::getKey(kn->m_data),k)));
    return(a);
  }


//*********
  virtual inorder_iterator erase(_iterator i) {
/*
** Iterator-Position hinter zu l�schendem
** Knoten ermitteln
** (Ohne Anfangs-Position f�r Iterator zu bestimmen)
*/
    inorder_iterator io(i.m_knot,this,0);
    ++io;
/*
** konnte Knoten gel�scht werden?
** => Position hinter gek�schtem Knoten zur�ckgeben
** Andernfalls End-Position zur�ckgeben
*/
    if(erase(reinterpret_cast<CAVLKnot*>(i.m_knot)))
      return(_iterator(io.m_knot,this,0));
    else
      return(inorder_end());
  }
//*********
  virtual inorder_iterator erase(inorder_iterator beg, inorder_iterator end) {
    while((beg!=inorder_end())&&(beg!=end)) {
      CKnot *kn=beg.m_knot;
      ++beg;
      erase(reinterpret_cast<CAVLKnot*>(kn));
    }
    return(beg);
  }

//*********
  virtual void pop_front() {
    if(!empty())
      erase(reinterpret_cast<CAVLKnot*>(inorder_begin().m_knot));
  }
//*********
  virtual void pop_back() {
    if(!empty())
      erase(reinterpret_cast<CAVLKnot*>(inorder_rbegin().m_knot));
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
  CAVLTree &operator=(const CAVLTree &t) {
    if(&t!=this) {

/*
** Alten Baum l�schen
*/
      deleteKnot(m_root);
      m_size=t.m_size;

/*
** Zuzuweisender Baum nicht leer?
** => alle Knoten kopieren
*/
      if(t.m_root) {

/*
** Zun�chst Wurzel kopieren
*/
        m_root=new CAVLKnot(0,0,0,t.m_root->m_data, reinterpret_cast<CAVLKnot*>(t.m_root)->m_balance);

/*
** S�hne der Wurzel mit copySons kopieren
*/
        copySons(reinterpret_cast<CAVLKnot*>(m_root), reinterpret_cast<CAVLKnot*>(t.m_root));
      }
    }
    return(*this);
  }
};



}; // namespace Codebook

#endif
