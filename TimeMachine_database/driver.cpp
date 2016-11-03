// file      : hello/driver.cxx
// copyright : not copyrighted - public domain

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <session.hxx>

#include "database.h" // create_database

#include "person.h"
#include "person_odb.h"

#include "source.h"
#include "source_odb.h"

#include "path.h"
#include "path_odb.h"

#include "file.h"
#include "file_odb.h"
using namespace std;
using namespace odb::core;
/*
void print (const file& f)
{
    cout << f.name() << endl
         << " source: " << f.sourceId()->name() << endl;
}*/


int main (int argc, char* argv[])
{
  try
    {
    auto_ptr<database> db (create_database (argc, argv));

    unsigned long testCam_id,path_id,file_id;
    //заполнение source

    {
     source MainCam ("MainCam","186.191.1.1");
     source SecondCam ("SecondCam","197.102.2.2");
     source ThirdCam ("ThirdCam","108.113.3.3");

     transaction t (db->begin ());

     testCam_id = db->persist(MainCam);
     testCam_id = db->persist(SecondCam);
     testCam_id = db->persist(ThirdCam);
     t.commit();}

    //вывод из source

    {
        typedef odb::query<source> query;
        typedef odb::result<source> result;

        transaction t (db->begin ());

        /*odb::result<source_stat> r (db->query<source_stat> ());
        const source_stat& ps (*r.begin ());

        cout << endl
             << "count  : " << ps.count  << endl
             << "name   : " << ps.name   << endl
             << "adress : " << ps.adress << endl;
        */
        result r (db->query<source> (query::name.is_not_null()));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            cout << i->name () << " "  << i->adress() << endl;
          //cout << i->first () << " " << i->last () << " " << i->age   () << endl;
        }
        t.commit ();
    }

    //заполнение path

   {
        path FirstPath ("C:/recordings/28.10.2016");
        path SecondPath ("C:/recordings/29.11.2017");
        path ThirdPath ("C:/recordings/30.12.2018");

        transaction t (db->begin ());

        path_id = db->persist(FirstPath);
        path_id = db->persist(SecondPath);
        path_id = db->persist(ThirdPath);
        t.commit();
    }

    //вывод из path

    {
        typedef odb::query<path> query;
        typedef odb::result<path> result;

        transaction t (db->begin ());

        result r (db->query<path> (query::filePath.is_not_null()));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            cout  << " path is: " << i->filePath() << endl;
          //cout << i->first () << " " << i->last () << " " << i->age   () << endl;
        }
        t.commit();
    }
    /*//заполнение file
   {

        file FirstFile ("1111",p1,s1);
        file SecondFile ("2222",2,2);
        file ThirdFile ("3333",2,2);

        transaction t (db->begin ());
        db->persist (p1);
        db->persist (s1);
        db->persist (FirstFile);
        file_id = db->persist(FirstFile);
        file_id = db->persist(SecondFile);
        file_id = db->persist(ThirdFile);
        t.commit();
    }

    //вывод из file

    {
        typedef odb::query<file> query;
        typedef odb::result<file> result;

        transaction t (db->begin ());

        result r (db->query<file> (query::pathId.is_not_null() && query::sourceId.is_not_null()));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            cout << i->name() << " path: " << i->pathId() << " source: " << i->sourceId() << endl;
          //cout << i->first () << " " << i->second () << " " << i->age   () << endl;
        }
        t.commit();
    }*/
    //заполнение file_new
    /*{
        shared_ptr<file> f (new file )
    }*/
}
  catch (const odb::exception& e)
  {
    cerr << e.what () << endl;
    return 1;
  }
}
