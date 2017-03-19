#ifndef MODELWRITERXMLHELPER_H
#define MODELWRITERXMLHELPER_H

class ModelItem;
class ModelMultiShapeItem;
//
class ModelNode;
class ModelCurve;
//
class ArrayModelItem;
//
class GraphModelVertex;
class GraphModelEdge;


class QGradient;
class QConicalGradient;
class QLinearGradient;
class QRadialGradient;
//
class QXmlStreamWriter;

#include <QObject>

/**
 * This class shouldn't normally exist.
 * But it finally does, due to code factorization.
 * For a detailled description, please read:
 *     http://stackoverflow.com/questions/41958380/c-design-pattern-strategy-save-load
 *
 * Note 1
 * ======
 * I didn't add
 *     virtual read (QXmlStreamReader &stream);
 *     virtual write(QXmlStreamWriter &stream);
 * to the ModelItem class since the use of xml stream is specific to
 * the xml reader/writer only. If I have added those virtual methods,
 * what if a Json reader/writer were to be implemented by another person?
 *
 * Note 2
 * ======
 * Another way to go around this issue is to use C/C++ macros,
 * each one defining the code snippet we want to be generic.
 * But I didn't do it that way because checking whether
 * the code is legitimate bacomes quite tedious.
 */

class ModelWriterXmlHelper : public QObject
{
    Q_OBJECT

public:
    /* Disable creation and copy (using C++11 syntax). */
    ModelWriterXmlHelper() = delete;
    ModelWriterXmlHelper(const ModelWriterXmlHelper&) = delete;
    ModelWriterXmlHelper& operator=(const ModelWriterXmlHelper&) = delete;
    ModelWriterXmlHelper(const ModelWriterXmlHelper&&) = delete;

    static void writeModelItemTo(QXmlStreamWriter &stream, const ModelItem &item);
    static void writeModelMultiShapeItemTo(QXmlStreamWriter &stream, const ModelMultiShapeItem &item);

    static void writeModelNodeTo(QXmlStreamWriter &stream, const ModelNode &node);
    static void writeModelCurveTo(QXmlStreamWriter &stream, const ModelCurve &curve,
                                  const QMap<ModelNode*, int> &nodeIndexes);

    static void writeArrayModelItemTo(QXmlStreamWriter &stream, const ArrayModelItem &item);

    static void writeGraphModelVertexTo(QXmlStreamWriter &stream, const GraphModelVertex &vertex);
    static void writeGraphModelEdgeTo(QXmlStreamWriter &stream, const GraphModelEdge &edge,
                                      const QMap<ModelNode*, int> &vertexIndexes);
};

#endif // MODELWRITERXMLHELPER_H
