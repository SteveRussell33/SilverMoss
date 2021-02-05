//
// Created by Kaushal Sali on 05/12/20.
//

#include "RaagEngine.h"

RaagEngine::RaagEngine() {
    initLastNotes();
}

void RaagEngine::initLastNotes() {
    m_lastNoteAroha = Note::NONE;
    for (auto it = m_aroha.crbegin(); it != m_aroha.crend(); it++) {
        if (!it->second.empty()) {
            m_lastNoteAroha = it->first;
            break;
        }
    }
    m_lastNoteAvroha = Note::NONE;
    for (auto it = m_avroha.cbegin(); it != m_avroha.cend(); it++) {
        if (!it->second.empty()) {
            m_lastNoteAvroha = it->first;
            break;
        }
    }
}

NoteGraph& RaagEngine::getAroha() {
    return m_aroha;
}

NoteGraph& RaagEngine::getAvroha() {
    return m_avroha;
}

Note RaagEngine::getCurrentNote() const {
    return m_currentNote;
}

int RaagEngine::getCurrentNoteAsMidi() const {
    return (m_currentOctave+1) * 12 + static_cast<int>(m_currentNote) + m_transposition;
}

int RaagEngine::getTransposition() const {
    return m_transposition;
}

void RaagEngine::setTransposition(int semitone) {
    m_transposition = semitone;
}

int RaagEngine::getCurrentOctave() const {
    return m_currentOctave;
}

void RaagEngine::setOctaveRange(int min, int max) {
    //TODO: This method should allow for setting octaves with offset (i.e. starting from arbitrary note instead of C)
    m_minOctave = min;
    m_maxOctave = std::max(min, max);   //TODO: Is this a good way to handle this?
    m_currentOctave = std::max(std::min(m_currentOctave, m_maxOctave), m_minOctave);
}

void RaagEngine::reset() {
    m_minOctave = 3;
    m_maxOctave = 6;
    m_currentOctave = 4;
    m_transposition = 0;
    m_currentNote = Note::Sa;
    m_aroha.reset();
    m_avroha.reset();
    initLastNotes();
}

bool RaagEngine::stepUp() {
    auto nextNotes = m_aroha.getConnectedNotes(m_currentNote);
    if (nextNotes.empty())
        return false;

    randomGen.setRange(0, static_cast<int>(nextNotes.size())-1);
    auto nextNote = nextNotes[randomGen.generate()];

    if(m_currentNote == m_lastNoteAroha && nextNote < m_currentNote)
        incOctave();

    m_currentNote = nextNote;
    return true;
}

bool RaagEngine::stepDown() {
    auto nextNotes = m_avroha.getConnectedNotes(m_currentNote);
    if (nextNotes.empty())
        return false;

    randomGen.setRange(0, static_cast<int>(nextNotes.size())-1);
    auto nextNote = nextNotes[randomGen.generate()];

    if(m_currentNote == m_lastNoteAvroha && nextNote > m_currentNote)
        decOctave();

    m_currentNote = nextNote;
    return true;
}

void RaagEngine::incOctave() {
    m_currentOctave++;
    if (m_currentOctave > m_maxOctave)
        m_currentOctave = m_minOctave;
}

void RaagEngine::decOctave() {
    m_currentOctave--;
    if (m_currentOctave < m_minOctave)
        m_currentOctave = m_maxOctave;
}

int RaagEngine::getMinOctave() const {
    return m_minOctave;
}

int RaagEngine::getMaxOctave() const {
    return m_maxOctave;
}

void RaagEngine::setCurrentOctave(int octave) {
    m_currentOctave = std::max(std::min(octave, m_maxOctave), m_minOctave);
}

void RaagEngine::setCurrentNote(Note note) {
    m_currentNote = note;
}
