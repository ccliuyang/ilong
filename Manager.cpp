#include "Manager.h"

Manager::Manager(ILong *parent) : QObject(parent),iLong(parent),sqlExcute(&parent->sqlExcute)
{
    QSqlQuery * query = sqlExcute->initLayerManager();
    while(query->next())
    {
        QString id = query->value(0).toString();
        QString name = query->value(1).toString();
        int visible = query->value(2).toInt();
        int selectable = query->value(3).toInt();
        loadLayer(id,name,visible ? true : false, selectable ? true : false);
    }
    delete query;
    query = 0;
}

QList<Layer *> Manager::getLayers()
{
    return list;
}

Layer *Manager::addLayer(QString name, QList<LayerFormat> *typeList)
{
    QString layerName = checkLayerName(name);
    Layer * layer  = new Layer(iLong, layerName, typeList);
    list.append(layer);
    return layer;
}

void Manager::removeLayer(QString name)
{
    for(int i=0; i<list.size(); i++)
    {
       if(list.at(i)->getLayerName() == name)
       {
           Layer * layer = list.at(i);
           delete layer;
           layer = 0;
           list.removeAt(i);
           return;
       }
    }
}

QString Manager::checkLayerName(QString name)
{
    QString tmp = name ;
    for(int i=0; i<list.size(); i++)
    {
        if(list.at(i)->getLayerName() == tmp)
        {
            tmp.append("*");
            break;
        }
    }
    return name  == tmp ? tmp : checkLayerName(tmp);
}

void Manager::loadLayer(QString id, QString name, bool visible, bool selectable)
{
    Layer * layer = new Layer(iLong,id,name,visible,selectable);
    list.append(layer);
}