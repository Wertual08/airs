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

    protected:
        DragActionType DragAction;

        virtual void OnMouseDown(MouseEventArgs& args) override
        {
            if (!args.Handled() && args.Key() == airs::Key::LButton && Contains(args.Position()))
            {
                Dragging = true;
                args.Handle();
            }
        }
        virtual void OnMouseUp(MouseEventArgs& args) override
        {
            if (Dragging) args.Handle();
            Dragging = false;
        }
        virtual void OnMouseMove(MouseEventArgs& args) override
        {
            if (Dragging)
            {
                vec2i delta = args.Delta();

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

                args.Handle();
            }
        }

    public:
        DragActionType GetDragAction() const { return DragAction; }
        void SetDragAction(DragActionType dat) { DragAction = dat; }
    };
}