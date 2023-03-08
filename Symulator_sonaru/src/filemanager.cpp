#include "filemanager.hh"
#include "qvectornd.h"
#include <QFile>
#include <QtXml>
#include <QTextStream>

FileManager::FileManager()
{

}

bool FileManager::saveSonarConf(QFile &file, Sonar const &sonar)
{
    QDomDocument xml("sonar_configuration");

    QDomElement root = xml.createElement("Sonar");

    root.setAttribute("Id", QString::number(sonar.get_id()));
    root.setAttribute("NumOfRec" ,QString::number(sonar.get_numOfRec()));
    root.setAttribute("Size", qVector3DtoQString(sonar.get_size()));
    root.setAttribute("LocPos", qVector3DtoQString(sonar.get_LocPos()));
    root.setAttribute("Color", qVector3DtoQString(sonar.get_color()));
    root.setAttribute("Rotation", QString::number(sonar.get_rotation()));
    root.setAttribute("GlPos", qVector3DtoQString(sonar.get_GlPos()));

    xml.appendChild(root);

    QDomElement transmitter = xml.createElement("Transmitter");
    root.appendChild(transmitter);
    transmitter.setAttribute("LocPos", qVector3DtoQString(sonar.transmitter_getLocPos()));
    transmitter.setAttribute("GlPos", qVector3DtoQString(sonar.transmitter_getGlPos()));
    transmitter.setAttribute("Size", qVector3DtoQString(sonar.transmitter_getSize()));
    transmitter.setAttribute("Rotation", QString::number(sonar.transmitter_getRotation()));
    transmitter.setAttribute("AcusticVec", qVector3DtoQString(sonar.getTransmitter().get_acusticVec()));

    QDomElement transmittedsignal = xml.createElement("Transmitted_signal");
    root.appendChild(transmittedsignal);
    TransmittedSignal const &signal = sonar.getTransmittedSignal();
    transmittedsignal.setAttribute("w", QString::number(signal.get_w()));
    transmittedsignal.setAttribute("p1", QString::number(signal.get_p1()));
    transmittedsignal.setAttribute("p2", QString::number(signal.get_p2()));
    transmittedsignal.setAttribute("tau1", QString::number(signal.get_tau1()));
    transmittedsignal.setAttribute("tau2", QString::number(signal.get_tau2()));
    transmittedsignal.setAttribute("M1", QString::number(signal.get_M1()));
    transmittedsignal.setAttribute("M2", QString::number(signal.get_M2()));
    transmittedsignal.setAttribute("Amp", QString::number(signal.get_Amp()));
    transmittedsignal.setAttribute("alpha", QString::number(signal.get_alpha()));
    transmittedsignal.setAttribute("time", QString::number(signal.get_time()));
    transmittedsignal.setAttribute("t_max", QString::number(signal.get_tMax()));

    for(uint i=0; i < sonar.get_numOfRec(); ++i){
        QDomElement Receiver = xml.createElement("Receiver");

        Receiver.setAttribute("Id", QString::number(sonar.rec_getId(i)));
        Receiver.setAttribute("Size", qVector3DtoQString(sonar.rec_getSize(i)));
        Receiver.setAttribute("Rotation", QString::number(sonar.rec_getRotation(i)));
        Receiver.setAttribute("LocPos", qVector3DtoQString(sonar.rec_getLocPos(i)));
        Receiver.setAttribute("GlPos", qVector3DtoQString(sonar.rec_getGlPos(i)));

        root.appendChild(Receiver);
    }

    QTextStream output(&file);
    output << xml.toString();
    qDebug() << "koniec";

    return true;
}

bool FileManager::readSonarConf(QFile &file, Sonar &sonar)
{

    QDomDocument xml;

    if(!xml.setContent(&file)){
        qDebug() << " Failed to parse the file into a DOM tree.";
        return false;
    }

    QDomElement root = xml.documentElement();
    if(root.tagName() != "Sonar"){
        qDebug() << "Sonar item not found";
        return false;
    }

    QString sonar_id = root.attribute("Id", "0");
    QString sonar_size = root.attribute("Size", "0 0 0");
    QString sonar_locpos = root.attribute("LocPos", "0 0 0");
    QString sonar_color = root.attribute("Color", "0 0 0");
    QString sonar_glpos = root.attribute("GlPos", "0 0 0");

    sonar.set_id(sonar_id.toUInt());
    sonar.set_LocPos(qStringToQVector3D(sonar_locpos));
    sonar.set_size(qStringToQVector3D(sonar_size));
    sonar.set_color(qStringToQVector3D(sonar_color));
    sonar.set_GlPos(qStringToQVector3D(sonar_glpos));

    QDomElement node = root.firstChild().toElement();

    if(node.tagName() == "Transmitter"){
        QString transmitter_locpos = node.attribute("LocPos", "0 0 0");
        QString transmitter_rotation = node.attribute("Rotation", "0");
        QString transmitter_size = node.attribute("Size", "0 0 0");
        QString transmitter_acusticVec = node.attribute("AcusticVec", "0 0 0");

        sonar.transmitter_setLocPos(qStringToQVector3D(transmitter_locpos));
        sonar.transmitter_setSize(qStringToQVector3D(transmitter_size));
        sonar.transmitter_setRotation(transmitter_rotation.toDouble());
        sonar.useTransmitter().set_acusticVec(qStringToQVector3D(transmitter_acusticVec));

    }
    node = node.nextSibling().toElement();
    if(node.tagName() == "Transmitted_signal"){
        TransmittedSignal &signal = sonar.useTransmittedSignal();
        QString signal_w = node.attribute("w", "0");
        QString signal_p1 = node.attribute("p1", "0");
        QString signal_p2 = node.attribute("p2", "0");
        QString signal_tau1 = node.attribute("tau1", "0");
        QString signal_tau2 = node.attribute("tau2", "0");
        QString signal_M1 = node.attribute("M1", "0");
        QString signal_M2 = node.attribute("M2", "0");
        QString signal_Amp = node.attribute("Amp", "0");
        QString signal_alpha = node.attribute("alpha", "0");
        QString signal_time = node.attribute("time", "0");
        QString signal_tMax = node.attribute("t_max", "0");

        signal.set_w(signal_w.toDouble());
        signal.set_p1(signal_p1.toDouble());
        signal.set_p2(signal_p2.toDouble());
        signal.set_tau1(signal_tau1.toDouble());
        signal.set_tau2(signal_tau2.toDouble());
        signal.set_M1(signal_M1.toDouble());
        signal.set_M2(signal_M2.toDouble());
        signal.set_Amp(signal_Amp.toDouble());
        signal.set_alpha(signal_alpha.toDouble());
        signal.set_time(signal_time.toDouble());
        signal.set_tMax(signal_tMax.toDouble());
    }
    node = node.nextSibling().toElement();
    while(node.isNull() == false){
        if(node.tagName() == "Receiver"){
            QString rec_id = node.attribute("Id", "0");
            QString rec_scale = node.attribute("Size", "0 0 0");
            QString rec_rot = node.attribute("Rotation", "0");
            QString rec_locpos = node.attribute("LocPos", "0 0 0");

            uint rec_id_value = rec_id.toUInt();
            QVector3D rec_scale_value = qStringToQVector3D(rec_scale);
            double rec_rot_value = rec_rot.toDouble();
            QVector3D rec_locpos_value = qStringToQVector3D(rec_locpos);

            sonar.addReceiver(rec_id_value, rec_scale_value, rec_rot_value, rec_locpos_value);
        }
        node = node.nextSibling().toElement();
   }

    return true;
}

bool FileManager::savePlatformConf(QFile &file, Platform const &platform)
{
    QDomDocument xml("platform_configuration");

    QDomElement root = xml.createElement("Platform");

    root.setAttribute("NumOfSonars", QString::number(platform.get_NumOfSonars()));
    root.setAttribute("Size", qVector3DtoQString(platform.get_size()));
    root.setAttribute("LocPos", qVector3DtoQString(platform.get_LocPos()));

    xml.appendChild(root);

    for(uint i=0; i < platform.get_NumOfSonars(); ++i){
        Sonar son = platform.getSonar(i);
        QDomElement sonar = xml.createElement("Sonar");
        sonar.setAttribute("LocPos", qVector3DtoQString(son.get_LocPos()));
        sonar.setAttribute("Color", qVector3DtoQString(son.get_color()));
        sonar.setAttribute("Id", QString::number(son.get_id()));
        sonar.setAttribute("Size", qVector3DtoQString(son.get_size()));
        sonar.setAttribute("Rotation", QString::number(son.get_rotation()));
        sonar.setAttribute("GlPos", qVector3DtoQString(son.get_GlPos()));

        root.appendChild(sonar);

        QDomElement transmitter = xml.createElement("Transmitter");
        transmitter.setAttribute("GlPos", qVector3DtoQString(son.transmitter_getGlPos()));
        transmitter.setAttribute("Size", qVector3DtoQString(son.transmitter_getSize()));
        transmitter.setAttribute("Rotation", QString::number(son.transmitter_getRotation()));
        transmitter.setAttribute("LocPos", qVector3DtoQString(son.transmitter_getLocPos()));
        transmitter.setAttribute("AcusticVec", qVector3DtoQString(son.getTransmitter().get_acusticVec()));

        sonar.appendChild(transmitter);

        QDomElement transmittedsignal = xml.createElement("Transmitted_signal");
        sonar.appendChild(transmittedsignal);
        TransmittedSignal const &signal = son.getTransmittedSignal();
        transmittedsignal.setAttribute("w", QString::number(signal.get_w()));
        transmittedsignal.setAttribute("p1", QString::number(signal.get_p1()));
        transmittedsignal.setAttribute("p2", QString::number(signal.get_p2()));
        transmittedsignal.setAttribute("tau1", QString::number(signal.get_tau1()));
        transmittedsignal.setAttribute("tau2", QString::number(signal.get_tau2()));
        transmittedsignal.setAttribute("M1", QString::number(signal.get_M1()));
        transmittedsignal.setAttribute("M2", QString::number(signal.get_M2()));
        transmittedsignal.setAttribute("Amp", QString::number(signal.get_Amp()));
        transmittedsignal.setAttribute("alpha", QString::number(signal.get_alpha()));
        transmittedsignal.setAttribute("time", QString::number(signal.get_time()));
        transmittedsignal.setAttribute("t_max", QString::number(signal.get_tMax()));

        for(uint i=0; i < son.get_numOfRec(); ++i){
            QDomElement receiver = xml.createElement("Receiver");

            receiver.setAttribute("Id", QString::number(son.rec_getId(i)));
            receiver.setAttribute("Size", qVector3DtoQString(son.rec_getSize(i)));
            receiver.setAttribute("Rotation", QString::number(son.rec_getRotation(i)));
            receiver.setAttribute("GlPos", qVector3DtoQString(son.rec_getGlPos(i)));
            receiver.setAttribute("LocPos", qVector3DtoQString(son.rec_getLocPos(i)));

            sonar.appendChild(receiver);
        }

    }

    QTextStream output(&file);
    output << xml.toString();

    return true;
}

bool FileManager::readPlatformConf(QFile &file, Platform &platform)
{
    QDomDocument xml;

    if(!xml.setContent(&file)){
        qDebug() << " Failed to parse the file into a DOM tree.";
        return false;
    }

    QDomElement root = xml.documentElement();
    if(root.tagName() != "Platform"){
        qDebug() << "Platform item not found";
        return false;
    }

    QString platfrm_size = root.attribute("Size", "0 0 0");
    QString platform_locpos = root.attribute("LocPos", "0 0 0");

    platform.set_size(qStringToQVector3D(platfrm_size));
    platform.set_LocPos(qStringToQVector3D(platform_locpos));


    QDomElement node = root.firstChild().toElement();

    while(!node.isNull()){
        Sonar sonar;
        if(node.tagName() == "Sonar"){
            QString sonar_id = node.attribute("Id", "0");
            QString sonar_scale = node.attribute("Size", "0 0 0");
            QString sonar_locpos = node.attribute("LocPos", "0 0 0");
            QString sonar_color = node.attribute("Color", "0 0 0");
            QString sonar_rotation = node.attribute("Rotation", "0");
            QString sonar_glpos = node.attribute("GlPos", "0 0 0");

            sonar.set_id(sonar_id.toUInt());
            sonar.set_LocPos(qStringToQVector3D(sonar_locpos));
            sonar.set_size(qStringToQVector3D(sonar_scale));
            sonar.set_color(qStringToQVector3D(sonar_color));
            sonar.set_rotation(sonar_rotation.toDouble());
            sonar.set_GlPos(qStringToQVector3D(sonar_glpos));
        }

        QDomElement sonarElem = node.firstChild().toElement();
        if(sonarElem.tagName() == "Transmitter"){
            QString transmitter_rotation = sonarElem.attribute("Rotation", "0");
            QString transmitter_glpos = sonarElem.attribute("GlPos", "0 0 0");
            QString transmitter_size = sonarElem.attribute("Size", "0 0 0");
            QString transmitter_locpos = sonarElem.attribute("LocPos", "0 0 0");
            QString transmitter_acusticVec = sonarElem.attribute("AcusticVec", "0 0 0");

            sonar.transmitter_setLocPos(qStringToQVector3D(transmitter_locpos));
            sonar.transmitter_setSize(qStringToQVector3D(transmitter_size));
            sonar.transmitter_setRotation(transmitter_rotation.toDouble());
            sonar.transmitter_setGlPos(qStringToQVector3D(transmitter_glpos));
            sonar.useTransmitter().set_acusticVec(qStringToQVector3D(transmitter_acusticVec));
        }
        sonarElem = sonarElem.nextSibling().toElement();

        if(sonarElem.tagName() == "Transmitted_signal"){
            TransmittedSignal &signal = sonar.useTransmittedSignal();
            QString signal_w = sonarElem.attribute("w", "0");
            QString signal_p1 = sonarElem.attribute("p1", "0");
            QString signal_p2 = sonarElem.attribute("p2", "0");
            QString signal_tau1 = sonarElem.attribute("tau1", "0");
            QString signal_tau2 = sonarElem.attribute("tau2", "0");
            QString signal_M1 = sonarElem.attribute("M1", "0");
            QString signal_M2 = sonarElem.attribute("M2", "0");
            QString signal_Amp = sonarElem.attribute("Amp", "0");
            QString signal_alpha = sonarElem.attribute("alpha", "0");
            QString signal_time = sonarElem.attribute("time", "0");
            QString signal_t_max = sonarElem.attribute("t_max", "0");

            signal.set_w(signal_w.toDouble());
            signal.set_p1(signal_p1.toDouble());
            signal.set_p2(signal_p2.toDouble());
            signal.set_tau1(signal_tau1.toDouble());
            signal.set_tau2(signal_tau2.toDouble());
            signal.set_M1(signal_M1.toDouble());
            signal.set_M2(signal_M2.toDouble());
            signal.set_Amp(signal_Amp.toDouble());
            signal.set_alpha(signal_alpha.toDouble());
            signal.set_time(signal_time.toDouble());
            signal.set_tMax(signal_t_max.toDouble());
        }

        sonarElem = sonarElem.nextSibling().toElement();

        while(!sonarElem.isNull()){
            if(sonarElem.tagName() == "Receiver"){
                QString rec_id = sonarElem.attribute("Id", "0");
                QString rec_size = sonarElem.attribute("Size", "0 0 0");
                QString rec_rot = sonarElem.attribute("Rotation", "0");
                QString rec_locpos = sonarElem.attribute("LocPos", "0 0 0");
                QString rec_glpos = sonarElem.attribute("GlPos", "0 0 0");

                uint rec_id_value = rec_id.toUInt();
                QVector3D rec_size_value = qStringToQVector3D(rec_size);
                double rec_rot_value = rec_rot.toDouble();
                QVector3D rec_locpos_value = qStringToQVector3D(rec_locpos);
                QVector3D rec_glpos_value = qStringToQVector3D(rec_glpos);
                sonar.addReceiver(rec_id_value, rec_size_value, rec_rot_value, rec_locpos_value, rec_glpos_value);
            }
            sonarElem = sonarElem.nextSibling().toElement();
        }
        platform.addSonar(sonar);
        node = node.nextSibling().toElement();
   }

    return true;
}

bool FileManager::saveScene(QFile &file, const SimulationScene &scene)
{
    QDomDocument xml("simulation_scene_configuration");
    QDomElement root = xml.createElement("Scene");

    root.setAttribute("NumOfPointObjects", QString::number(scene.points.size()));
    root.setAttribute("NumOfStakeObjects", QString::number(scene.poles.size()));
    root.setAttribute("NumOfFlatSurfaceObjects", QString::number(scene.flats.size()));

    xml.appendChild(root);
    for(const auto &point : scene.points){
        QDomElement pointElem = xml.createElement("Point");
        pointElem.setAttribute("Pos", qVector3DtoQString(point.position));
        pointElem.setAttribute("Color", qVector3DtoQString(point.color));
        pointElem.setAttribute("Size", QString::number(point.size));

        root.appendChild(pointElem);
    }
    for(const auto &pole : scene.poles){
        QDomElement poleElem = xml.createElement("Stake");
        poleElem.setAttribute("Pos", qVector3DtoQString(pole.position));
        poleElem.setAttribute("Color", qVector3DtoQString(pole.color));
        poleElem.setAttribute("Size", qVector3DtoQString(pole.size));

        root.appendChild(poleElem);
    }
    for(const auto &flat : scene.flats){
        QDomElement flatElem = xml.createElement("FlatSurface");
        flatElem.setAttribute("Pos", qVector3DtoQString(flat.position));
        flatElem.setAttribute("Color", qVector3DtoQString(flat.color));
        flatElem.setAttribute("Size", qVector3DtoQString(flat.size));

        root.appendChild(flatElem);
    }

    QTextStream output(&file);
    output << xml.toString();
    return true;
}

bool FileManager::readSceneConf(QFile &file, SimulationScene &scene)
{
    QDomDocument xml;

    if(!xml.setContent(&file)){
        qDebug() << " Failed to parse the file into a DOM tree.";
        return false;
    }

    QDomElement root = xml.documentElement();
    if(root.tagName() != "Scene"){
        qDebug() << "Scene item not found";
        return false;
    }

    QDomElement node = root.firstChild().toElement();
    while(node.isNull() == false){
        if(node.tagName() == "Point"){
            QString color = node.attribute("Color", "0 0 0");
            QString size = node.attribute("Size", "0");
            QString pos = node.attribute("Pos", "0 0 0");
            PointObject object;
            object.color = qStringToQVector3D(color);
            object.position = qStringToQVector3D(pos);
            object.size = size.toDouble();
            scene.points.push_back(object);
        }
        if(node.tagName() == "Stake"){
            QString color = node.attribute("Color", "0 0 0");
            QString size = node.attribute("Size", "0 0 0");
            QString pos = node.attribute("Pos", "0 0 0");
            PoleObjects object;
            object.color = qStringToQVector3D(color);
            object.position = qStringToQVector3D(pos);
            object.size = qStringToQVector3D(size);
            scene.poles.push_back(object);
        }
        if(node.tagName() == "FlatSurface"){
            QString color = node.attribute("Color", "0 0 0");
            QString size = node.attribute("Size", "0 0 0");
            QString pos = node.attribute("Pos", "0 0 0");
            FlatObjects object;
            object.color = qStringToQVector3D(color);
            object.position = qStringToQVector3D(pos);
            object.size = qStringToQVector3D(size);
            scene.flats.push_back(object);
        }
        node = node.nextSibling().toElement();
    }

    return true;
}

QVector3D FileManager::qStringToQVector3D(QString str)
{
    std::istringstream istrm;
    istrm.str(str.toStdString());

    double x,y,z;
    istrm >> x >> y >> z;

    if(istrm.fail()){
        return QVector3D(0,0,0);
    }

    return QVector3D(x, y, z);
}

QString FileManager::qVector3DtoQString(QVector3D vec)
{
    return QString("%1 %2 %3").arg(vec[0]).arg(vec[1]).arg(vec[2]);
}
