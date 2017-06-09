#include "SyntaxHighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
}

void SyntaxHighlighter::registerRule(const SyntaxHighlighter::SyntaxHighlightingRule &rule)
{
    m_syntaxHighlightingRules.append(rule);
}

void SyntaxHighlighter::registerRule(const QTextCharFormat &format, const QRegExp &regex)
{
    registerRule({format, regex});
}

void SyntaxHighlighter::registerRule(const QTextCharFormat &format, const QString &regexPattern)
{
    registerRule(format, QRegExp(regexPattern));
}

void SyntaxHighlighter::registerRules(const QTextCharFormat &format, const QList<QRegExp> &regexes)
{
    for(const auto &regex : regexes) {
        registerRule(format, regex);
    }
}

void SyntaxHighlighter::registerRules(const QTextCharFormat &format, const QStringList &regexPatterns)
{
    for(const auto &pattern : regexPatterns) {
        registerRule(format, pattern);
    }
}

void SyntaxHighlighter::clearRules()
{
    m_syntaxHighlightingRules.clear();
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    for(const SyntaxHighlightingRule &rule : m_syntaxHighlightingRules) {
        highlightBlock(text, rule);
    }
}

void SyntaxHighlighter::highlightBlock(const QString &text, const SyntaxHighlighter::SyntaxHighlightingRule &rule)
{
    const QTextCharFormat &format(rule.format);
    const QString &pattern(rule.regex.pattern()); // if regex is used directly, the program stucks.

    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while(index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, format);
        index = text.indexOf(expression, index + length);
    }
}
