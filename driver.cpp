
/*{
  try
{       QDateTime firstDateTime(QDate(2011,1,1),QTime(1,10,00));
        for (qint32 i = 1; i < 100; ++i)
        {
            insert(firstDateTime = firstDateTime.addSecs(1),"frstfl","CAM01","192.186.1.1","/storage/strg",i);
        }
        QDateTime secondDateTime(QDate(2022,2,2),QTime(02,22,00));
        for (qint32 i = 1; i < 100; ++i)
        {
            insert(secondDateTime = secondDateTime.addSecs(1),"scndfile","CAM02","192.222.2.2","/storage/ctrg",i);
        }
        QDateTime thirdDateTime(QDate(2033,3,3),QTime(03,33,00));
        for (qint32 i = 1; i < 100; ++i)
        {
           insert(thirdDateTime = thirdDateTime.addSecs(1),"thrdfile","CAM03","193.333.3.3","/storage/thrtg",i);
        }

        select(firstDateTime.addSecs(-11));
        select(secondDateTime.addSecs(-22));
        select(thirdDateTime.addSecs(-33));
}
  catch (const odb::exception& e)
  {
    cerr << e.what () << endl;
    return 1;
  }
}*/
