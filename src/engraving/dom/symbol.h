/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MU_ENGRAVING_SYMBOL_H
#define MU_ENGRAVING_SYMBOL_H

#include <memory>

#include "modularity/ioc.h"
#include "draw/types/font.h"

#include "iengravingfontsprovider.h"

#include "bsymbol.h"

namespace mu::engraving {
class Segment;
class IEngravingFont;

//---------------------------------------------------------
//   @@ Symbol
///    Symbol constructed from builtin symbol.
//
//   @P symbol       string       the SMuFL name of the symbol
//---------------------------------------------------------

class Symbol : public BSymbol
{
    OBJECT_ALLOCATOR(engraving, Symbol)
    DECLARE_CLASSOF(ElementType::SYMBOL)

    INJECT(IEngravingFontsProvider, engravingFonts)

public:
    Symbol(const ElementType& type, EngravingItem* parent, ElementFlags f = ElementFlag::MOVABLE);
    Symbol(EngravingItem* parent, ElementFlags f = ElementFlag::MOVABLE);
    Symbol(const Symbol&);

    Symbol& operator=(const Symbol&) = delete;

    Symbol* clone() const override { return new Symbol(*this); }

    void setSym(SymId s, const std::shared_ptr<IEngravingFont>& sf = nullptr) { m_sym  = s; m_scoreFont = sf; }
    SymId sym() const { return m_sym; }
    const std::shared_ptr<IEngravingFont>& scoreFont() const { return m_scoreFont; }
    mu::AsciiStringView symName() const;

    String accessibleInfo() const override;

    PropertyValue getProperty(Pid) const override;
    bool setProperty(Pid, const PropertyValue&) override;

    double baseLine() const override { return 0.0; }
    virtual Segment* segment() const { return (Segment*)explicitParent(); }

protected:
    SymId m_sym = SymId::noSym;
    std::shared_ptr<IEngravingFont> m_scoreFont = nullptr;
};

//---------------------------------------------------------
//   @@ FSymbol
///    Symbol constructed from a font glyph (i.e. a text character or emoji).
//---------------------------------------------------------

class FSymbol final : public BSymbol
{
    OBJECT_ALLOCATOR(engraving, FSymbol)
    DECLARE_CLASSOF(ElementType::FSYMBOL)

public:
    FSymbol(EngravingItem* parent);
    FSymbol(const FSymbol&);

    FSymbol* clone() const override { return new FSymbol(*this); }

    String toString() const;
    String accessibleInfo() const override;

    double baseLine() const override { return 0.0; }
    Segment* segment() const { return (Segment*)explicitParent(); }
    const mu::draw::Font& font() const { return m_font; }
    char32_t code() const { return m_code; }
    void setFont(const mu::draw::Font& f);
    void setCode(char32_t val) { m_code = val; }

private:
    mu::draw::Font m_font;
    char32_t m_code = 0; // character code point (Unicode)
};
} // namespace mu::engraving
#endif
