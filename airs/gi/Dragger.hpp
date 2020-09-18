#pragma once
#include "Element.hpp"



namespace airs::gi
{
    class Dragger : public Element
    {
    public:
        enum DragActionType : std::int32_t
        {
            None = 0b0,
            Drag = 0b1,
            DragOwner = 0b10,
            ResizeOwnerU = 0b100,
            ResizeOwnerL = 0b1000,
            ResizeOwnerD = 0b10000,
            ResizeOwnerR = 0b100000,
        };

    private:
        bool Dragging = false;
        airs::vec2i MousePos;

    protected:
        DragActionType DragAction;

    public:
        DragActionType GetDragAction() const { return DragAction; }
        void SetDragAction(DragActionType dat) { DragAction = dat; }

        virtual bool OnMouseDown(vec2i pos, key key) override
        {
            if (key == airs::key::LButton && Contains(pos))
            {
                Dragging = true;
                MousePos = pos;
                return true;
            }
            else return false;
        }
        virtual bool OnMouseUp(vec2i pos, key key) override
        {
            bool result = Dragging;
            Dragging = false;
            return result;
        }
        virtual bool OnMouseMove(vec2i pos) override
        {
            if (Dragging)
            {
                vec2i delta = pos - MousePos;
                MousePos = pos;

                if (DragAction & Drag) SetPosition(GetPosition() + delta);
                if (GetOwner())
                {
                    Element* owner = GetOwner();
                    if (DragAction & DragOwner) owner->SetPosition(owner->GetPosition() + delta);
                    if (DragAction & ResizeOwnerR) owner->SetExtent(owner->GetExtent() + vec2i(delta.x, 0));
                    if (DragAction & ResizeOwnerU) owner->SetExtent(owner->GetExtent() + vec2i(0, delta.y));
                    if (DragAction & ResizeOwnerL)
                    {
                        owner->SetPosition(owner->GetPosition() + vec2i(delta.x, 0));
                        owner->SetExtent(owner->GetExtent() - vec2i(delta.x, 0));
                    }
                    if (DragAction & ResizeOwnerD)
                    {
                        owner->SetPosition(owner->GetPosition() + vec2i(0, delta.y));
                        owner->SetExtent(owner->GetExtent() - vec2i(0, delta.y));
                    }
                }

                return true;
            }
            else return false;
        }
    };
}