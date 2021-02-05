
// ------------------------------------------------------------------------------------------------------------
#include "NoteGraph.h"

NoteGraph::NoteGraph() {
    for (int i=0; i<static_cast<int>(Note::TOTAL); i++) {
        m_graph.insert(std::pair<Note, std::vector<Note>>(static_cast<Note>(i), std::vector<Note>()));

    }
}

const std::vector<Note>& NoteGraph::getConnectedNotes(Note from) {
    return m_graph[from];
}

void NoteGraph::connect(Note from, Note to) {
    m_graph[from].push_back(to);
}

void NoteGraph::disconnect(Note from, Note to) {
    for (auto it=m_graph[from].cbegin(); it != m_graph[from].cend(); it++) {
        if (*it == to) {
            m_graph[from].erase(it);
            return;
        }
    }
}

void NoteGraph::disconnectAll(Note from) {
    m_graph[from].clear();
}

void NoteGraph::reset() {
    for(auto& note : m_graph)
        note.second.clear();
}

Graph::const_iterator NoteGraph::cbegin() {
    return m_graph.cbegin();
}

Graph::const_iterator NoteGraph::cend() {
    return m_graph.cend();
}

Graph::const_reverse_iterator NoteGraph::crbegin() {
    return m_graph.crbegin();
}

Graph::const_reverse_iterator NoteGraph::crend() {
    return m_graph.crend();
}

/*
 * Notes:
 *
 * unordered_set is used which forces connections to unique notes. This avoids accidentlly creating a lot of duplicate
 * entries (multiple connections between same notes). In case user connects two patch cords from note A to Note B in
 * two separate input extention modules, only one connection will be considered. Later A probability based
 * implementation can be used for Note where counts of the connections are stored.
 *
 */