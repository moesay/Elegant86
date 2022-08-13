#include <assembler/include/base.h>
#include <algorithm>
#include <cctype>
#include <iterator>
#include <qnamespace.h>

/*
 * -- I/P -> Hex [with 0x prefix], O/P -> Hex Str
 * -- I/P -> Dec, O/P -> Hex Str
 *
 *  A decimal conversion interface is provided to eliminate confusion.
 */

QString Base::numToHexStr (const std::integral auto &param, OutputSize opSize, Sign sign)
{
    std::stringstream ss;

    if(sign == Sign::Pos) {
        if(param > 0xFF) {
            ss << std::hex << ((param & 0x00f0) >> 4);
            ss << std::hex << ((param & 0x000f) >> 0);
            ss << std::hex << ((param & 0xf000) >> 12);
            ss << std::hex << ((param & 0x0f00) >> 8);
        } else {
            ss << std::hex << ((param & 0x00f0) >> 4);
            ss << std::hex << ((param & 0x000f) >> 0);
        }
    }
    else {
        if(param > 0xFF) {
            ss << std::hex << ((param & 0xf000) >> 12);
            ss << std::hex << ((param & 0x0f00) >> 8);
            ss << std::hex << ((param & 0x00f0) >> 4);
            ss << std::hex << ((param & 0x000f) >> 0);
        } else {
            ss << std::hex << ((param & 0x00f0) >> 4);
            ss << std::hex << ((param & 0x000f) >> 0);
        }
    }

    if(opSize != OutputSize::Dynamic) {
        if(opSize == OutputSize::Byte) {
            if(sign == Sign::Pos)
                while(ss.str().length() < 2) ss << "0";
            else if(sign == Sign::Neg)
                while(ss.str().length() < 2) ss << "F";

            if(ss.str().size() > 2) {
                ss.str(ss.str().substr(0, 2));
            }
        }
        else if(opSize == OutputSize::Word) {
            if(sign == Sign::Pos)
                while(ss.str().length() < 4) ss << "0";
            else if(sign == Sign::Neg)
                while(ss.str().length() < 4) ss << "F";

            if(ss.str().size() > 4)
                ss.str(ss.str().substr(ss.str().size() - 4, ss.str().size()));
        }
    }

    if (ss.str().length() % 2) {
        if(sign == Sign::Pos)
            return "0" + QString::fromStdString(ss.str()).toUpper();
        else
            return "F" + QString::fromStdString(ss.str()).toUpper();
    }

    return QString::fromStdString(ss.str()).toUpper();
}

QString Base::numToHexStr (const QString &param, OutputSize opSize, Sign sign) {
    if(isDecValue(param))
        return numToHexStr(param.toInt(0, 10), opSize, sign);
    else if(isHexValue(param))
        return numToHexStr(param.toInt(0, 16), opSize, sign);
    else if(isBinValue(param))
        return numToHexStr(param.chopped(1).toInt(0, 2), opSize, sign);

    return "";
}


bool Base::isMemAddr(const QString &param) {
    if(param.startsWith('[') && param.endsWith(']'))
        return true;
    return false;
}

std::optional<std::tuple<QString, QString, QString>> Base::tokenize() {
    pointerType = Pointer::None;
    QStringList list;

    if(tokens == 3)
        list = codeLine.split(QRegExp(" |\\,"), Qt::SplitBehaviorFlags::SkipEmptyParts);
    else if(tokens == 2)
        list = codeLine.split(QRegExp(" "), Qt::SplitBehaviorFlags::SkipEmptyParts);

    if(list.at(list.length()-1).compare("WPTR", Qt::CaseSensitivity::CaseInsensitive) == 0  ||
            list.at(list.length()-1).compare("BPTR", Qt::CaseSensitivity::CaseInsensitive) == 0)
        return std::nullopt;

    for(int i = 0; i < list.length(); i++) {
        QString temp = list.at(i);
        if(list.at(i).compare("WPTR", Qt::CaseSensitivity::CaseInsensitive) == 0) {
            if(getOperandType(list.at(i+1)) == OperandType::Immed8 || getOperandType(list.at(i+1)) == OperandType::Immed16)
                return std::nullopt;
            pointerType = Pointer::Word;
            list.removeAt(i);
        }
        else if(list.at(i).compare("BPTR", Qt::CaseSensitivity::CaseInsensitive) == 0) {
            if(getOperandType(list.at(i+1)) == OperandType::Immed8 || getOperandType(list.at(i+1)) == OperandType::Immed16)
                return std::nullopt;
            pointerType = Pointer::Byte;
            list.removeAt(i);
        }

        //digits hwnd
        if(isDecValue(list.at(i)))
            list[i] = "0X" + numToHexStr(list.at(i));
        else if(isBinValue(list.at(i)))
            list[i] = "0X" + numToHexStr(list.at(i));
        else if(list.at(i).endsWith('h', Qt::CaseInsensitive) && isHexValue(list.at(i)))
            list[i] = "0X" + list.at(i).chopped(1);

        //long immed hwnd
        if(isLongImmed(list.at(i))) {
            if(list.at(i).endsWith("h", Qt::CaseInsensitive))
                list[i] = list.at(i).chopped(1);
            list[i] = "0X" + list.at(i).mid(list.at(i).length() - 4, 4);
        }
    }
    if(tokens == 2) {
        if(list.count() < 2) return std::nullopt;
        return std::make_tuple(list.at(0).toUpper(), list.at(1).toUpper(), "");
    }
    else if(tokens == 3) {
        if(list.count() < 3) return std::make_tuple(list.at(0).toUpper(), list.at(1).toUpper(), "");
        else return std::make_tuple(list.at(0).toUpper(), list.at(1).toUpper(), list.at(2).toUpper());
    }

    return std::nullopt;
}

bool Base::hasSegmentPrefix(const QString& param) {
    for(const auto &p : SegRegs) {
        if(param.toUpper().startsWith(p+':'))
            return true;
    }
    return false;
}

QString Base::getSegmentPrefix(const QString& param) {
    return param.toUpper().split(':').at(0);
}

QString Base::stripSegmentPrefix(const QString& param) {
    if(hasSegmentPrefix(param))
        return param.split(':').at(1);
    return param;
}

bool Base::segmentPrefixWrapper(QString& firstOp) {
    if(hasSegmentPrefix(firstOp)) {
        machineCode.append(numToHexStr(getSegRegPrefix(getSegmentPrefix(firstOp))));
        firstOp = stripSegmentPrefix(firstOp);
        return true;
    }
    return false;
}

bool Base::segmentPrefixWrapper(QString& firstOp, QString& secondOp) {
    if(hasSegmentPrefix(firstOp) && hasSegmentPrefix(secondOp))
        throw InvalidSegmentOverridePrefix();
    else if(hasSegmentPrefix(firstOp)) {
        machineCode.append(numToHexStr(getSegRegPrefix(getSegmentPrefix(firstOp))));
        firstOp = stripSegmentPrefix(firstOp);
        return true;
    }
    else if(hasSegmentPrefix(secondOp)) {
        machineCode.append(numToHexStr(getSegRegPrefix(getSegmentPrefix(secondOp))));
        secondOp = stripSegmentPrefix(secondOp);
        return true;
    }
    return false;
}

enum OperandType Base::getOperandType(const QString& operand) {
    QString strippedOperand = stripSegmentPrefix(operand);
    if(isChar(strippedOperand)) return OperandType::Char;
    else if(Regs8.contains(strippedOperand.trimmed().toUpper())) return OperandType::Reg8;
    else if(Indexers.contains(strippedOperand.trimmed().toUpper())) return OperandType::Indexer;
    else if(Regs16.contains(strippedOperand.trimmed().toUpper())) return OperandType::Reg16;
    else if(SegRegs.contains(strippedOperand.trimmed().toUpper())) return OperandType::SegReg;
    else if(strippedOperand.trimmed().toUpper() == "SHORT") return OperandType::JShort;
    else if(strippedOperand.trimmed().toUpper() == "LONG") return OperandType::JLong;
    else if(strippedOperand.trimmed().toUpper() == "FAR") return OperandType::JFar;
    else if(strippedOperand.trimmed() == "") return OperandType::Empty;
    else if(isMemAddr(strippedOperand)) return OperandType::MemAddr;
    else if(Labels::labelExists(strippedOperand)) return OperandType::Label;
    else if(isImmed8(strippedOperand)) return (strippedOperand.toInt(nullptr, 16) >= 0 ? OperandType::Immed8 : OperandType::NegImmed8);
    else if(isImmed16(strippedOperand)) return (strippedOperand.toInt(nullptr, 16) >= 0 ? OperandType::Immed16 : OperandType::NegImmed16);
    else if(isLongImmed(strippedOperand)) return(strippedOperand.toInt(nullptr, 16) >= 0 ? OperandType::LongImmed : OperandType::NegLongImmed);
    return OperandType::Invalid;
}

uchar Base::rmGenerator(const QString& param) {
    QString addr = param.trimmed().toUpper();

    QStringList argList = addr.split(QRegExp("[+]"), Qt::SplitBehaviorFlags::SkipEmptyParts);
    if(argList.size() >= 2) {
        if(argList.contains("BX") && argList.contains("SI")) return mod00["BX+SI"];
        else if(argList.contains("BX") && argList.contains("DI")) return mod00["BX+DI"];
        else if(argList.contains("BP") && argList.contains("SI")) return mod00["BP+SI"];
        else if(argList.contains("BP") && argList.contains("DI")) return mod00["BP+DI"];
        else if(argList.contains("BX") && std::none_of(std::begin(addressingRegs), std::end(addressingRegs),
                    [argList](const QString& p) {return argList.contains(p) && p != "BX";})) return mod00["BX"];
        else if(argList.contains("DI") && std::none_of(std::begin(addressingRegs), std::end(addressingRegs),
                    [argList](const QString& p) {return argList.contains(p) && p != "DI";})) return mod00["DI"];
        else if(argList.contains("SI") && std::none_of(std::begin(addressingRegs), std::end(addressingRegs),
                    [argList](const QString& p) {return argList.contains(p) && p != "SI";})) return mod00["SI"];
    }
    else if(argList.size() == 1) {
        if(argList.contains("BX")) return mod00["BX"];
        else if(argList.contains("SI")) return mod00["SI"];
        else if(argList.contains("DI")) return mod00["DI"];
        else return mod00["DA"];
    }
    //on error
    return 0xFF;
}

bool Base::isChar(const QString& param) {
    return ((param.length() == 3) &&
            (param.startsWith("'")) &&
            (param.endsWith("'")));
}

bool Base::isImmed8(const QString& param) {
    QString t = param;
    bool b;
    if(param.endsWith('h', Qt::CaseInsensitive)) t = param.chopped(1);
    return isHexValue(param) && abs(t.toLongLong(&b, 16)) <= 0xFF && b;
}

bool Base::isImmed16(const QString& param) {
    QString t = param;
    bool b;
    if(param.endsWith('h', Qt::CaseInsensitive)) t = param.chopped(1);
    return isHexValue(param) && (abs(t.toLongLong(&b, 16)) > 0xFF && abs(t.toLongLong(&b, 16)) <= 0xFFFF) && b;
}

bool Base::isLongImmed(const QString& param) {
    QString t = param;
    bool b;
    if(param.endsWith('h', Qt::CaseInsensitive)) t = param.chopped(1);
    return isHexValue(t) && abs(param.toLongLong(&b, 16)) > 0xFFFF && b;
}

void Base::setCodeLine(const std::string& param) {
    codeLine = QString::fromStdString(param);
}

void Base::setCodeLine(const QString& param) {
    codeLine = param;
}

const QString& Base::getCodeLine() { return codeLine; }

uchar Base::modRegRmGenerator(const uchar& mod, const uchar& reg, const uchar& rm) {
    return ((mod << 6) | (reg << 3) | (rm));
}

uchar Base::getGpRegCode(const QString& param, OperandType ot, bool *ok) {
    std::unordered_map<std::string, uchar>::iterator match;
    if(ot == OperandType::Reg16)
        match = Regs16Hex.find(param.toUpper().toStdString());
    else if(ot == OperandType::Reg8)
        match = Regs8Hex.find(param.toUpper().toStdString());
    if(match != std::end(Regs16Hex)) {
        if(ok != nullptr) *ok = false;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

QString Base::extractDisplacment(const QString& param, bool *ok) {
    QStringList splittedAddress = param.split(QRegExp("[+]"));
    QStringList filteredDisplacment = splittedAddress.filter(QRegularExpression("[0-9a-fA-F]"));

    if(filteredDisplacment.size() > 0) {
        filteredDisplacment.first().remove(']'); filteredDisplacment.first().remove('[');
        if(ok != nullptr) *ok=true;
        return filteredDisplacment.first();
    }
    if(ok != nullptr) *ok = false;
    return "";
}

uchar Base::getSegRegPrefix(const QString& param, bool *ok) {
    auto match = segRegsPrefix.find(param.toUpper().toStdString());
    if(match != std::end(segRegsHex)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

uchar Base::getSegRegCode(const QString& param, bool *ok) {
    auto match = segRegsHex.find(param.toUpper().toStdString());
    if(match != std::end(segRegsHex)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0x00;
}

bool Base::isHexValue(const QString& param) {
    std::string hex = param.toStdString();

    std::transform(std::begin(hex), std::end(hex), std::begin(hex), ::toupper);
    if(hex.starts_with("0X")) {
        hex.erase(0, 2);
        if(std::all_of(std::begin(hex), std::end(hex), ::isxdigit)) return true;
    }
    else if(hex.ends_with("H")) {
        hex.erase(hex.size()-1, 1);
        if(std::all_of(std::begin(hex), std::end(hex), ::isxdigit)) return true;
    }

    return false;
}

void Base::hexValidator(QStringList& param) {
    for(auto &disp : param) {
        if(isBinValue(disp))
            disp = "0X" + numToHexStr(disp);
        else if(isDecValue(disp))
            disp = "0X" + numToHexStr(disp);
    }
    param.erase(
            std::remove_if(
                std::begin(param),
                std::end(param),
                [this](QString const &p) {return !this->isHexValue(p);}), std::end(param));
}

std::optional<QString> Base::signHandler(const QString& param, const OperandType& ot) {
    if(ot == OperandType::NegImmed8) {
        if(abs(param.toInt(nullptr, 16)) > 0xFE) {
            uint16_t hexVal = param.toInt(nullptr, 16);
            return numToHexStr(hexVal, OutputSize::Dynamic, Sign::Neg);
        }
        else {
            uint8_t hexVal = param.toInt(nullptr, 16);
            return numToHexStr(hexVal, OutputSize::Dynamic, Sign::Neg);
        }
    }
    else if(ot == OperandType::NegImmed16) {
        if(abs(param.toInt(nullptr, 16)) > 0x7FFF) return std::nullopt;
        uint16_t hexVal = param.toInt(nullptr, 16);
        return numToHexStr(hexVal, OutputSize::Dynamic, Sign::Neg);
    }
    return std::nullopt;
}

bool Base::isDecValue(const QString& param) {
    bool b;
    for(const auto& c : param) {
        if(!c.isDigit()) return false;
    }
    param.toLongLong(&b, 10);
    return true & b;
}

bool Base::isBinValue(const QString& param) {
    bool b = std::all_of(std::begin(param), std::end(param) -1, [](QChar x) {return (x == '0' || x == '1');});
    return b && param.endsWith('b', Qt::CaseInsensitive);
}

/*
 * Should've been implemented using regex but, who cares. i hate regex.
 * Maybe later
 */
bool Base::isLableDef(QString lbl) {
    /* if(getOperandType */
    bool init = (lbl.startsWith('.') && lbl.endsWith(':'));
    lbl = lbl.left(lbl.length() -1);
    lbl = lbl.right(lbl.length() -1);
    return (!lbl.contains('.') && !lbl.contains(':') && lbl.length() > 0 && init);
}


void Base::setInfo(const info_t& param) {
    this->info = param;
}

bool Base::isSegOffset(const QString& param) {
    QStringList temp = param.split(':');
    if(temp.size() != 2) return false;
    bool seg = isDecValue(temp.at(0)) || isBinValue(temp.at(0)) || isHexValue(temp.at(0));
    bool offset = isDecValue(temp.at(1)) || isBinValue(temp.at(1)) || isHexValue(temp.at(1));

    return (seg & offset);
}

/*
 * The Mod byte (other that 0x03) is specifyied by the length of the displacment.
 * But it's not always the case, a smart assembler will produce as short assembly code as possible.
 * If the number is within the range that F-extending it will give the same value. then strip the F's and
 * set the Mod to be 0x01, otherwise, the Mod is 0x02
 * Mod 0x03 is for registers and 0x00 has nothing to do with displacment. It only works when its a direct address.
 * @Known Bug : The Mod for negative numbers is miscalculated
 */
bool Base::isMod1(const int &displacmentValue) {
    return ((displacmentValue >= 0x0000 && displacmentValue <= 0x007F) ||
            (displacmentValue >= 0xFF80 && displacmentValue < 0xFFFF));
}
