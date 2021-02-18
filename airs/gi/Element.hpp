#pragma once
#include <cstdint>
#include <vector>
#include <airs/math.hpp>
#include <airs/KeyCodes.hpp>
#include <airs/gi/Renderer.hpp>
#include <airs/gi/EventArgs.hpp>



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
        std::uint32_t ID;
        airs::vec2i Position;
        airs::vec2i Extent;
        Constraint Constraints;
        bool Resizable;

        Element* Owner;
        std::vector<Element*> Elements;

        bool Master;

    protected:
        virtual void OnMouseDown(MouseEventArgs& args) { }
        virtual void OnMouseUp(MouseEventArgs& args) { }
        virtual void OnMouseWheel(MouseEventArgs& args) { }
        virtual void OnMouseMove(MouseEventArgs& args) { }
        virtual void OnKeyDown(KeyboardEventArgs& args) { }
        virtual void OnKeyUp(KeyboardEventArgs& args) { }
        virtual void OnChar(KeyboardEventArgs& args) { }

    public:
        Element() :
            ID(0),
            Resizable(false),
            Owner(nullptr),
            Master(false)
        {
        }
        Element(Element&& element) noexcept
        {
            std::swap(ID, element.ID);
            std::swap(Position, element.Position);
            std::swap(Extent, element.Extent);
            std::swap(Constraints, element.Constraints);
            std::swap(Resizable, element.Resizable);

            std::swap(Owner, element.Owner);
            std::swap(Elements, element.Elements);

            std::swap(Master, element.Master);
        }
        ~Element()
        {
            if (Master)
                for (auto elem : Elements)
                    delete elem;
        }

        bool GetMaster() const { return Master; }
        void SetMaster(bool master) { Master = master; }

        std::uint32_t GetID() const { return ID; }
        virtual void SetID(std::uint32_t id) { ID = id; }

        airs::vec2i GetPosition() const { return Position; }
        virtual void SetPosition(airs::vec2i pos) 
        { 
            Position = pos;
            for (Element* element : Elements) element->ApplyConstraints(Position, Extent);
        }

        airs::vec2i GetExtent() const { return Extent; }
        virtual void SetExtent(airs::vec2i ext) 
        { 
            Extent = ext;
            for (Element* element : Elements) element->ApplyConstraints(Position, Extent);
        }

        Constraint GetConstraints() const { return Constraints; }
        virtual void SetConstraints(Constraint con) { Constraints = con; }
        virtual void SetConstraintU(Anchor anc, std::int32_t off) { Constraints.AnchorU = anc; Constraints.OffsetU = off; }
        virtual void SetConstraintL(Anchor anc, std::int32_t off) { Constraints.AnchorL = anc; Constraints.OffsetL = off; }
        virtual void SetConstraintD(Anchor anc, std::int32_t off) { Constraints.AnchorD = anc; Constraints.OffsetD = off; }
        virtual void SetConstraintR(Anchor anc, std::int32_t off) { Constraints.AnchorR = anc; Constraints.OffsetR = off; }

        bool GetResizable() const { return Resizable; }
        virtual void SetResizable(bool res) { Resizable = res; }

        Element* GetOwner() const { return Owner; }
        virtual void SetOwner(Element* own) { Owner = own; }

        void Add(Element* element)
        {
            element->SetOwner(this);
            element->ApplyConstraints(GetPosition(), GetExtent());
            Elements.push_back(element);
        }
        void Remove(Element* element)
        {
            std::size_t j = 0;
            for (std::size_t i = 0; i < Elements.size(); i++)
                if (Elements[i] != element) Elements[j++] = Elements[i];
            Elements.resize(j);
        }
        std::size_t Size() const { return Elements.size(); }
        Element* operator[](std::size_t i) const { return Elements[i]; }


        bool Contains(airs::vec2i pos) const { return pos >= Position && pos < Position + Extent; }

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

            for (Element* element : Elements) element->ApplyConstraints(Position, Extent);
        }

        void SendMouseDown(MouseEventArgs& args)
        { 
            std::size_t i = Elements.size();
            while (i--) Elements[i]->SendMouseDown(args);
            OnMouseDown(args);
        }
        void SendMouseUp(MouseEventArgs& args)
        {
            std::size_t i = Elements.size();
            while (i--) Elements[i]->SendMouseUp(args);
            OnMouseUp(args);
        }
        void SendMouseWheel(MouseEventArgs& args)
        {
            std::size_t i = Elements.size();
            while (i--) Elements[i]->SendMouseWheel(args);
            OnMouseWheel(args);
        }
        void SendMouseMove(MouseEventArgs& args)
        {
            std::size_t i = Elements.size();
            while (i--) Elements[i]->SendMouseMove(args);
            OnMouseMove(args);
        }
        void SendKeyDown(KeyboardEventArgs& args)
        {
            std::size_t i = Elements.size();
            while (i--) Elements[i]->SendKeyDown(args);
            OnKeyDown(args);
        }
        void SendKeyUp(KeyboardEventArgs& args)
        {
            std::size_t i = Elements.size();
            while (i--) Elements[i]->SendKeyUp(args);
            OnKeyUp(args);
        }
        void SendChar(KeyboardEventArgs& args)
        {
            std::size_t i = Elements.size();
            while (i--) Elements[i]->SendChar(args);
            OnChar(args);
        }

        virtual void Render(Renderer& renderer) 
        { 
            for (Element* element : Elements) element->Render(renderer); 
        }
    };
}