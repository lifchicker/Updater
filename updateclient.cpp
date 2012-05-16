#include "updateclient.h"

UpdateClient::UpdateClient(QObject *parent) :
    QObject(parent)
{
}

bool UpdateClient::copyFile(const QString &locationFrom, const QString &locationTo, const QString &fileName)
{
}

bool UpdateClient::copyFile(const QString &locationFrom, const QString &locationTo, const QString &originalFileName, const QString &resultFileName)
{
}

bool UpdateClient::copyFiles(const QString &source, const QString &destination)
{
}

QString UpdateClient::downloadFile(const QUrl &)
{
}

bool UpdateClient::moveFile(const QString &locationFrom, const QString &locationTo, const QString &fileName)
{
}

bool UpdateClient::moveFile(const QString &locationFrom, const QString &locationTo, const QString &originalFileName, const QString &resultFileName)
{
}

UpdateFile UpdateClient::readHeader(const QString &)
{
}

void UpdateClient::readListOfFiles(const UpdateFile &)
{
}

QString UpdateClient::readMainBinaryName(const QString &)
{
}


/*
    Parses the XML data and creates treeWidget items accordingly.
*/
void UpdateClient::parseXml()
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "item")
                linkString = xml.attributes().value("rss:about").toString();
            currentTag = xml.name().toString();
        } else if (xml.isEndElement()) {
            if (xml.name() == "item") {

                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, titleString);
                item->setText(1, linkString);
                treeWidget->addTopLevelItem(item);

                titleString.clear();
                linkString.clear();
            }

        } else if (xml.isCharacters() && !xml.isWhitespace()) {
            if (currentTag == "title")
                titleString += xml.text().toString();
            else if (currentTag == "link")
                linkString += xml.text().toString();
        }
    }
    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
    }
}
