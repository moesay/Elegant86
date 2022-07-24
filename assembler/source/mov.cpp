#include <assembler/include/base.h>
#include <assembler/include/mov.h>

InstRet_T Mov::process() {
    machineCode.clear();
    /*
     * All ::process() functions of Elegant86 works the same way
     *  1- Tokenize the ::codeLine into mnemonic, src, dest.
     *  2- The correct ::process() function will always be triggered for the very specific mnemonic
     *      so, there is no need to check for the mnemonic. This is left to be done in the main function.
     *  3- Check for the operands type, and upon that, do something.
     */

    std::optional<std::tuple<QString, QString, QString>> temp = tokenize();

    if(temp == std::nullopt)
        return {"", false, "Invalid Pointer"};
    auto [mnemonic, dest, src] = *temp;

    try {
        /*
         * The following function handels the segment prefix overriding
         * It do so by removing the segment and inserting its opcode into ~machineCode
         */
        segmentPrefixWrapper(dest, src);
    } catch(InvalidSegmentOverridePrefix& exc) {
        return {"", false, exc.what()};
    }

    //get the operand type. is it a reg16, reg8, mem address, segreg or what?
    destType = getOperandType(dest);  srcType = getOperandType(src);

    /*
     * The ::getOperandType() is limited to only one input, and in many cases we need to lazy-check for the type
     * as some types depends on the the other type. The memory is one of these types.
     * A generic memory type (MemAddr) is assigned to any memory address (any thing grouped in square brackets)
     * and in this function, depending on the type of the other operand, the memory length specifier and the memory length (if immed addressing is involved)
     * the memory type is altered
     */
    if(destType == OperandType::MemAddr) {
        //The Pointer::Null means that this is a memory address, but no bptr or wptr were invloved
        if(pointerType != Pointer::None)
            pointerType == Pointer::Byte ? destType = OperandType::Mem8 : destType = OperandType::Mem16;
        else {
            if(srcType == OperandType::Reg16 || srcType == OperandType::SegReg)
                destType = OperandType::Mem16;
            else if(srcType == OperandType::Reg8)
                destType = OperandType::Mem8;
            else
                return {"", false, "Unknown pointer size"};
        }
    }
    else if(srcType == OperandType::MemAddr) {
        if(pointerType != Pointer::None)
            pointerType == Pointer::Byte ? srcType = OperandType::Mem8 : srcType = OperandType::Mem16;
        else {
            if(destType == OperandType::Reg16 || destType == OperandType::SegReg || destType == OperandType::Indexer) //add the indexer to the rest
                srcType = OperandType::Mem16;
            else if(destType == OperandType::Reg8)
                srcType = OperandType::Mem8;
            else
                return {"", false, "Unknown pointer size"};
        }
    }

    /*
     * Its fine to use a 16bit regs as a dest when to src is an 8 bit immed. value. just extend it
     * But there's no opcode for it so here we change its type to be an immed16 and the ::numToHexStr() function will extend it.
     */
    if(destType == OperandType::Mem16 && srcType == OperandType::Immed8)
        srcType = Immed16;
    if(srcType == OperandType::LongImmed) srcType = OperandType::Immed16;
    if(srcType == OperandType::Immed16 && destType == OperandType::Mem8)
        srcType = OperandType::Immed8;

    //in the form of [destType]-[srcType]
    generalExpression = Operands[destType] + '-' + Operands[srcType];

    /*
     * When both operands are of the same type, the processing is done by fetching the opcode and
     * building the ModRegRM byte that represents the src and the dest
     */
    if(((destType==OperandType::Reg16) && (srcType==OperandType::Reg16)) ||
            ((destType==OperandType::Reg8) && (srcType==OperandType::Reg8))) {
        //the mod is indeed 3, and the machine code is 2 bytes
        //the opcode, and the modregrm byte
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));
        /*
         * Mod is always 0x03 == 11b
         * Reg is the src register
         * RM is the dest register
         */
        machineCode.append(numToHexStr(modRegRmGenerator(0X3, getGpRegCode(src, srcType), getGpRegCode(dest, destType))));
        return {machineCode, true, ""};
    }

    /*
     * The 8086 programmers manual states that each regsiter has its own opcode for immed values moving
     * There is no general opcode that takes r16-IMMED. So instead, we have to get to opcode using the name of the register
     * stored in ~dest
     */
    else if(destType==OperandType::Reg8 && (srcType==OperandType::Immed8 || srcType == OperandType::Immed16)) {
        opcode = getOpcode(dest+"-i8", &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));
        machineCode.append(numToHexStr(src, OutputSize::Byte));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Reg16 && (srcType==OperandType::Immed16 || srcType==Immed8)){
        opcode = getOpcode(dest+"-i16", &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode)); //the instruction machine code, in the case its mov XX, XX is any 8 bit reg
        machineCode.append(numToHexStr(QString(src), OutputSize::Word));

        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Mem8 || srcType==OperandType::Mem8 ||
            destType==OperandType::Mem16 || srcType==OperandType::Mem16) {

        //Which operand is the memory address?
        bool destIsMemAddr = destType == OperandType::Mem8 || destType == OperandType::Mem16;
        /*
         * The memory address can contain some registers and each of them corresponds to a specific RM byte. So we have to split it
         * to be able to check it
         */
        QStringList addrArgs;
        uchar reg;

        //Catch the memory address operand that is marked by the angle brackets. Strip the brackets.
        if(destIsMemAddr) {
            dest.remove("["); dest.remove("]");
            addrArgs = dest.split(QRegExp("[+]"));
        } else {
            src.remove("["); src.remove("]");
            addrArgs = src.split(QRegExp("[+]"));
        }

        //The address may contain some displacment, extract it.
        QStringList displacment = addrArgs.filter(QRegularExpression("[0-9a-fA-F]"));
        if(std::any_of(std::begin(addrArgs), std::end(addrArgs), [](const QString& p) {
                    return (notAdressingRegs.contains(p) || Regs8.contains(p));
                    })) return {"", false, "Invalid addressing register/s"};
        //Validate the displacment list against any empty enteries or NaN values.
        hexValidator(displacment);

        /*
         * If the length of the address after splitting it == 1, then its a direct address.
         * We must check for such a case because the direct addresses has their own Mod byte.
         */
        bool directAddress = addrArgs.size() == 1;
        int displacmentValue = 0;

        //If there is something in the ~displacment, then its indeed a displacment that we care about. take it out of the QStringList and store it as an int.
        if(displacment.size() >= 1)
            displacmentValue = displacment.first().toInt(nullptr, 16);

        if(displacment.empty()) mod = 0x00;         //for bx+si, bp+di ... and si, di as well
        else if(!displacment.empty()) {
            if(displacment.size() == 1 && addrArgs.size() == 1) mod = 0x00; //direct address;
            else mod = (isMod1(displacmentValue) ? 0x01 : 0x02);       //the rest of the cases!
        }

        /*
         * The accum. has its own opcodes for dealing with memory, so we have to check if the
         * src or the dest are ax or al.
         */
        if((dest == "AL" || dest == "AX") && isHexValue(src)) {
            uchar opcode = getOpcode(dest +'-'+ Operands[srcType], &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(numToHexStr(opcode));
            machineCode.append(numToHexStr(extractDisplacment(src), OutputSize::Word));
            return {machineCode, true, ""};
        }
        if((src == "AL" || src == "AX") && isHexValue(dest)) {
            uchar opcode = getOpcode(Operands[destType] +'-'+ src, &state);
            if(state == false) return {"", false, "Invalid Operands"};
            machineCode.append(numToHexStr(opcode));
            machineCode.append(numToHexStr(extractDisplacment(dest), OutputSize::Word));
            return {machineCode, true, ""};
        }

        /*
         * If the dest is the memaddr, the src has some posiblilites.
         * We have to check for them individually becasue the hex code of each registers set is stored in a separate container.
         */
        if(destIsMemAddr) {
            if(srcType == OperandType::SegReg)
                reg = getSegRegCode(src);
            else if(srcType == OperandType::Reg16 || srcType == OperandType::Reg8)
                reg = getGpRegCode(src, srcType);
            else if(srcType == OperandType::Indexer) {
                reg = indexersHex.find(src.toStdString())->second;
                generalExpression = "m16-r16"; //treating indexers the same way i treat reg16 as they share the same opcode. TBC.
            }
            //The src might be an immed value
            else if(srcType == Immed8 || srcType == Immed16)
                reg = 0X00;
            uchar rm = rmGenerator(dest);
            if(rm == 0xFF) return {"", false, "Unhandeled Error"};
            modregrm = modRegRmGenerator(mod, reg, rm);
        } else {
            if(destType == OperandType::SegReg)
                reg = getSegRegCode(dest);
            else if(destType == OperandType::Reg16 || destType == OperandType::Reg8)
                reg = getGpRegCode(dest, destType);
            else if(destType == OperandType::Indexer) {
                reg = indexersHex.find(dest.toStdString())->second;
                generalExpression = "r16-m16"; //treating indexers the same way i treat reg16 as they share the same opcode. TBC.
            }
            //if the source is a memaddr, the dest cant be an immid. value
            else
                reg = getGpRegCode(dest, destType);
            uchar rm = rmGenerator(src);
            if(rm == 0xFF) return {"", false, "Unhandeled Error"};
            modregrm = modRegRmGenerator(mod, reg, rm);
        }

        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));
        machineCode.append(numToHexStr(modregrm));
        if(!displacment.empty())
            machineCode.append(numToHexStr(displacment.first(), ((directAddress || mod == 0x02) ? OutputSize::Word : OutputSize::Byte)));
        if(srcType == OperandType::Immed8)
            machineCode.append(numToHexStr(src, OutputSize::Byte));
        else if (srcType == OperandType::Immed16)
            machineCode.append(numToHexStr(src, OutputSize::Word));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::Reg16 && srcType==OperandType::SegReg) {
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        modregrm = modRegRmGenerator(0x03, getSegRegCode(src), getGpRegCode(dest, destType));
        machineCode.append(numToHexStr(opcode)); machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }

    else if(destType==OperandType::SegReg && srcType==OperandType::Reg16) {
        opcode = getOpcode(generalExpression, &state);
        if(state == false) return {"", false, "Invalid Operands"};
        modregrm = modRegRmGenerator(0x03, getSegRegCode(dest), getGpRegCode(src, srcType));
        machineCode.append(numToHexStr(opcode)); machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }

    else if(destType == OperandType::Reg16 && srcType == OperandType::Indexer) {
        opcode = getOpcode("r16-r16", &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));
        auto indexerCode = indexersHex.find(src.toStdString())->second;
        modregrm = modRegRmGenerator(0x3, indexerCode, getGpRegCode(dest, OperandType::Reg16));
        machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }

    else if(destType == OperandType::Indexer && srcType == OperandType::Reg16) {
        opcode = getOpcode("r16-r16", &state);
        if(state == false) return {"", false, "Invalid Operands"};
        machineCode.append(numToHexStr(opcode));
        auto indexerCode = indexersHex.find(dest.toStdString())->second;
        modregrm = modRegRmGenerator(0x3, getGpRegCode(src, OperandType::Reg16), indexerCode);
        machineCode.append(numToHexStr(modregrm));
        return {machineCode, true, ""};
    }
    return {"", false, "Invalid Operands"};
}

uchar Mov::getOpcode(const QString& param, bool *ok) {
    auto match = LUT.find(param.toStdString());
    if(match != std::end(LUT)) {
        if(ok != nullptr) *ok = true;
        return match->second;
    }
    if(ok != nullptr) *ok = false;
    return 0xF1; //not used
}

Mov::Mov(const QString& param) {
    tokens = 3;
    this->setCodeLine(param);
}
Mov::Mov() {tokens = 3;}
