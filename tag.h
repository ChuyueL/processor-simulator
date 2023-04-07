#ifndef TAG_H
#define TAG_H

enum FUType {
    ARITH,
    BRANCH,
    LOADSTORE,
    NONE
};

class Tag {
    public: 

    FUType FU_type;
    int number;

    Tag(FUType _FU_type, int _number) {
        FU_type = _FU_type;
        number = _number;
    }

    bool equals(Tag other_tag);
};

class PlaceholderTag : public Tag {
    public:
    PlaceholderTag() : Tag(NONE, 0) {
    }
};

#endif