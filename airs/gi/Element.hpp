#pragma once
#include <cstdint>
#include "../math.hpp"
#include "../KeyCodes.hpp"
#include "Renderer.hpp"



namespace airs::gi
{
    class Element
    {
    public:
        enum Anchor : std::int32_t
        {
            None,
            Lesser,
            Larger,
            Middle,
        };
        struct Constraint
        {
            Anchor AnchorU = None;
            Anchor AnchorL = None;
            Anchor AnchorD = None;
            Anchor AnchorR = None;
            std::int32_t OffsetU = 0;
            std::int32_t OffsetL = 0;
            std::int32_t OffsetD = 0;
            std::int32_t OffsetR = 0;
        };

    private:
        //GIEventRing* EventRing;

        std::uint32_t ID;
        airs::vec2i Position;
        airs::vec2i Extent;
        Constraint Constraints;
        bool Resizable;
        Element* Owner;

    protected:
        //bool PushEvent(const GIEvent& e) { return EventRing->Push(e); }

    public:
        Element() :
            //EventRing(nullptr),
            ID(0),
            Resizable(false),
            Owner(nullptr)
        {
        }

        //void SetEventRing(GIEventRing* ring) { EventRing = ring; }

        std::uint32_t GetID() const { return ID; }
        virtual void SetID(std::uint32_t id) { ID = id; }

        airs::vec2i GetPosition() const { return Position; }
        virtual void SetPosition(airs::vec2i pos) { Position = pos; }

        airs::vec2i GetExtent() const { return Extent; }
        virtual void SetExtent(airs::vec2i ext) { Extent = ext; }

        Constraint GetConstraints() const { return Constraints; }
        virtual void SetConstraints(Constraint con) { Constraints = con; }
        Constraint GetConstraintU() const { return Constraints; }
        virtual void SetConstraintU(Anchor anc, std::int32_t off) { Constraints.AnchorU = anc; Constraints.OffsetU = off; }
        Constraint GetConstraintL() const { return Constraints; }
        virtual void SetConstraintL(Anchor anc, std::int32_t off) { Constraints.AnchorL = anc; Constraints.OffsetL = off; }
        Constraint GetConstraintD() const { return Constraints; }
        virtual void SetConstraintD(Anchor anc, std::int32_t off) { Constraints.AnchorD = anc; Constraints.OffsetD = off; }
        Constraint GetConstraintR() const { return Constraints; }
        virtual void SetConstraintR(Anchor anc, std::int32_t off) { Constraints.AnchorR = anc; Constraints.OffsetR = off; }

        bool GetResizable() const { return Resizable; }
        virtual void SetResizable(bool res) { Resizable = res; }

        Element* GetOwner() const { return Owner; }
        virtual void SetOwner(Element& own) { Owner = &own; }

        virtual bool IsContainer() const { return false; }

        bool Contains(airs::vec2i pos) const { return pos >= Position && pos < Position + Extent; }

        virtual bool OnMouseDown(airs::vec2i pos, airs::key key) { return false; }
        virtual bool OnMouseUp(airs::vec2i pos, airs::key key) { return false; }
        virtual bool OnMouseWheel(airs::vec2i pos, float delta) { return false; }
        virtual bool OnMouseHWheel(airs::vec2i pos, float delta) { return false; }
        virtual bool OnMouseMove(airs::vec2i pos) { return false; }
        virtual bool OnKeyDown(airs::key key) { return false; }
        virtual bool OnKeyUp(airs::key key) { return false; }
        virtual bool OnChar(char32_t c) { return false; }

        void ApplyConstraints(airs::vec2i pos, airs::vec2i ext)
        {
            switch (Constraints.AnchorL)
            {
            case Middle: Position.x = pos.x + ext.x / 2 + Constraints.OffsetL; break;
            case Lesser: Position.x = pos.x + Constraints.OffsetL; break;
            case Larger: Position.x = pos.x + ext.x + Constraints.OffsetL; break;
            }
            if (Constraints.AnchorL != None)
            {
                switch (Constraints.AnchorR)
                {
                case Middle: Extent.x = pos.x + ext.x / 2 + Constraints.OffsetR - Position.x; break;
                case Lesser: Extent.x = pos.x + Constraints.OffsetR - Position.x; break;
                case Larger: Extent.x = pos.x + ext.x + Constraints.OffsetR - Position.x; break;
                }
            }
            else
            {
                switch (Constraints.AnchorR)
                {
                case Middle: Position.x = pos.x + ext.x / 2 + Constraints.OffsetR - Extent.x; break;
                case Lesser: Position.x = pos.x + Constraints.OffsetR - Extent.x; break;
                case Larger: Position.x = pos.x + ext.x + Constraints.OffsetR - Extent.x; break;
                }
            }

            switch (Constraints.AnchorD)
            {
            case Middle: Position.y = pos.y + ext.y / 2 + Constraints.OffsetD; break;
            case Lesser: Position.y = pos.y + Constraints.OffsetD; break;
            case Larger: Position.y = pos.y + ext.y + Constraints.OffsetD; break;
            }
            if (Constraints.AnchorD != None)
            {
                switch (Constraints.AnchorU)
                {
                case Middle: Extent.y = pos.y + ext.y / 2 + Constraints.OffsetU - Position.y; break;
                case Lesser: Extent.y = pos.y + Constraints.OffsetU - Position.y; break;
                case Larger: Extent.y = pos.y + ext.y + Constraints.OffsetU - Position.y; break;
                }
            }
            else
            {
                switch (Constraints.AnchorU)
                {
                case Middle: Position.y = pos.y + ext.y / 2 + Constraints.OffsetU - Extent.y; break;
                case Lesser: Position.y = pos.y + Constraints.OffsetU - Extent.y; break;
                case Larger: Position.y = pos.y + ext.y + Constraints.OffsetU - Extent.y; break;
                }
            }
        }

        virtual void Render(Renderer& renderer) { }
    };
}