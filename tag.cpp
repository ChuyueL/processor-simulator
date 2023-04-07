#include "tag.h"

bool Tag::equals(Tag other_tag) {
    return (FU_type == other_tag.FU_type && number == other_tag.number);
}