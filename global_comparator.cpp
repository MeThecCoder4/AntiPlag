#include "global_comparator.h"

using namespace std;
using namespace Qt;

GlobalComparator::GlobalComparator(const QString &text1, const QString &text2)
    : m_text1(text1), m_text2(text2) {}

int GlobalComparator::compare()
{
    int coincidence = 0;
    map<QString, int> map1, map2;

    if((!(map1 = mapIdentifiers(m_text1.toUpper())).empty()) &&
       (!(map2 = mapIdentifiers(m_text2.toUpper())).empty()))
        coincidence = calculateCoincidence(map1, map2);

    return coincidence;
}

QStringList GlobalComparator::getUniqueWordsList()
{
    return m_uniqueWordsList;
}

// Find all identifiers in text and count their occurances
map<QString, int> GlobalComparator::mapIdentifiers(const QString &text)
{
    if(text == nullptr || text.length() == 0)
        return map<QString, int>();

    QStringList identifiers = text.split(QRegExp("[^A-Z0-9]+"));
    map<QString, int> mapOfIdentifiers;

    for(const auto& id : identifiers)
        mapOfIdentifiers[id]++;

    return mapOfIdentifiers;
}

// This function checks how many words are the same in both texts (match is case insensitive)
// and returns the percentage of matched words
int GlobalComparator::calculateCoincidence(const std::map<QString, int> &map1, const std::map<QString, int> &map2)
{
    m_uniqueElements.clear();
    m_uniqueWordsList.clear();

    for(const auto& entry : map1)
        if(!entry.first.isEmpty())
            m_uniqueElements.insert(entry.first);

    for(const auto& key : m_uniqueElements) {
        auto entry1 = map1.find(key);
        auto entry2 = map2.find(key);

        if(entry1 != map1.end() && entry2 != map2.end()) {
            if(entry1->second == entry2->second) {
                m_uniqueWordsList << entry1->first;
            }
        }
    }

    return static_cast<int>((static_cast<float>(m_uniqueWordsList.size()) / m_uniqueElements.size()) * 100);
}
