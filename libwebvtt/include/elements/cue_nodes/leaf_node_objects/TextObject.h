#ifndef LIBWEBVTT_TEXT_OBJECT_H
#define LIBWEBVTT_TEXT_OBJECT_H

#include "elements/cue_nodes/LeafNodeObject.h"

namespace WebVTT
{
    class TextObject : public LeafNodeObject
    {
    public:
        TextObject(std::u32string_view input)
        {
            this->text = input;
        }
        virtual NodeObject::NodeType getNodeType() const;
        void accept(ICueTreeVisitor &visitor) override;

    private:
        std::u32string text;
    };

} // namespace WebVTT

#endif //LIBWEBVTT_TEXT_OBJECT_H