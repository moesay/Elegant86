#include "highliter.h"
#include <QDebug>
Highliter::Highliter(QTextDocument *parent) : QSyntaxHighlighter(parent) {

    st_HighlightRule rule;

    numbersFmt.setForeground(Qt::darkCyan);
    rule.pattern = QRegularExpression(QStringLiteral("[0-9a-fA-F]"));
    rule.format = numbersFmt;
    hRulesVec.append(rule);

    mnemonicsFmt.setForeground(Qt::blue);
    mnemonicsFmt.setFontItalic(true);
    const QString mnemonics[] { "\\bmov\\b",
        "\\badd\\b", "\\bush\\b", "\\bpop\\b", "\\bxchg\\b", "\\bin\\b", "\\bout\\b", "\\badc\\b",
        "\\binc\\b", "\\bsub\\b", "\\bsbb\\b", "\\bdec\\b", "\\bneg\\b", "\\bcmp\\b", "\\baas\\b",
        "\\bdas\\b", "\\bmul\\b", "\\bimul\\b", "\\baam\\b", "\\bdiv\\b", "\\bidiv\\b", "\\baad\\b",
        "\\bcbw\\b", "\\bcwd\\b", "\\bnot\\b", "\\bshl\\b", "\\bsal\\b", "\\bshr\\b", "\\bsar\\b",
        "\\brol\\b", "\\bror\\b", "\\brcl\\b", "\\brcr\\b", "\\band\\b", "\\btest\\b", "\\bor\\b",
        "\\bxor\\b", "\\brep\\b", "\\bmovs\\b", "\\bcmps\\b", "\\bscas\\b", "\\blods\\b", "\\bstds\\b",
        "\\bcall\\b", "\\bjmp\\b", "\\bret\\b", "\\bje\\b", "\\bjz\\b", "\\bjl\\b", "\\bjnge\\b", "\\bjle\\b",
        "\\bjng\\b", "\\bjb\\b", "\\bjnae\\b", "\\bjbe\\b", "\\bjna\\b", "\\bjp\\b", "\\bjpe\\b", "\\bjo\\b",
        "\\bjs\\b", "\\bjne\\b", "\\bjnz\\b", "\\bjnl\\b", "\\bjge\\b", "\\bjnle\\b", "\\bjg\\b", "\\bjnb\\b",
        "\\bjae\\b", "\\bjnbe\\b", "\\bja\\b", "\\bjnp\\b", "\\bjpo\\b", "\\bjno\\b", "\\bret\\b", "\\bjns\\b",
        "\\bloop\\b", "\\bloopz\\b", "\\bloope\\b", "\\bloopnz\\b", "\\bloopne\\b", "\\bjcxz\\b", "\\bint\\b",
        "\\binto\\b", "\\biret\\b", "\\bclc\\b", "\\bcmc\\b", "\\bstc\\b", "\\bcld\\b", "\\bstd\\b", "\\bcli\\b",
        "\\bsti\\b", "\\bhlt\\b", "\\bwait\\b", "\\besc\\b", "\\block\\b", "\\bsegment\\b"
    };
    for (const QString &word : mnemonics) {
        rule.format = mnemonicsFmt;
        rule.pattern = QRegularExpression(word);
        rule.pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        hRulesVec.append(rule);
    }

    regsFmt.setForeground(Qt::darkMagenta);
    const QString regs[] { "\\bax\\b",
        "\\bbx\\b", "\\bcx\\b", "\\bdx\\b", "\\bah\\b", "\\bal\\b",
        "\\bbh\\b", "\\bbl\\b", "\\bch\\b", "\\bcl\\b", "\\dh\\n", "\\bdl\\b",
        "\\bsp\\b", "\\bbp\\b", "\\bsi\\b", "\\bdi\\b", "\\bes\\b", "\\bcs\\b",
        "\\bss\\b", "\\bds\\b"};
    for (const QString &word : regs) {
        rule.format = regsFmt;
        rule.pattern = QRegularExpression(word);
        rule.pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

        hRulesVec.append(rule);
    }

    commentsFmt.setForeground(Qt::green);
    commentsFmt.setBackground(Qt::black);
    rule.format = commentsFmt;
    rule.pattern = QRegularExpression(QStringLiteral(";[^\n]*"));
    hRulesVec.append(rule);


    stringsFmt.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = stringsFmt;
    hRulesVec.append(rule);
}

void Highliter::highlightBlock(const QString& text) {
    for(const st_HighlightRule &rule : qAsConst(hRulesVec)) {
        QRegularExpressionMatchIterator matchIter = rule.pattern.globalMatch(text);
        while(matchIter.hasNext()) {
            QRegularExpressionMatch match = matchIter.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

