#include "setting.hpp"

#include <QDebug>

SettingDataItem::SettingDataItem(ModelItem *parent)
    : ESMDataItem(parent)
{}

SettingDataItem::~SettingDataItem()
{}

void SettingDataItem::load(ESM::ESMReader &esm)
{
    //FIXME WTF
    mSetting.id = mId.toStdString();

    mSetting.load(esm);
}

QString SettingDataItem::key()
{
    return QString::fromStdString(mSetting.id);
}

QVariant SettingDataItem::value()
{
    switch(mSetting.type) {
    case ESM::VT_None:
        return QVariant(QVariant::Invalid);
    case ESM::VT_String:
        return QVariant(QString::fromStdString(mSetting.str));
    case ESM::VT_Int:
        return QVariant(mSetting.i);
    case ESM::VT_Float:
        return QVariant(mSetting.f);
    default:
        qDebug() << "Unexpected data type" << mSetting.type;
        return QVariant();
    };
}