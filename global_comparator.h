#ifndef GLOBAL_COMPARATOR_H
#define GLOBAL_COMPARATOR_H

#include <map>
#include <set>
#include <QString>
#include <QStringList>

class GlobalComparator {
public:
    GlobalComparator();

    GlobalComparator(const QString& text1, const QString& text2);

    int compare();

    QStringList getUniqueWordsList();

private:
    std::map<QString, int> mapIdentifiers(const QString& text);

    int calculateCoincidence(const std::map<QString, int>& map1, const std::map<QString, int>& map2);

    QString m_text1;

    QString m_text2;

    std::set<QString> m_uniqueElements;

    QStringList m_uniqueWordsList;

};

#endif // GLOBAL_COMPARATOR_H
