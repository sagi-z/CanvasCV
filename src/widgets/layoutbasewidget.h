#ifndef LAYOUTBASEWIDGET_H
#define LAYOUTBASEWIDGET_H

#include "layoutbase.h"
#include "compoundwidget.h"

namespace canvascv
{

/**
 * @brief The LayoutBaseWidget class
 * Base class for all Layout managers which are also widgets (not the Canvas class)
 */
class LayoutBaseWidget : public LayoutBase, public CompoundWidget
{
public:

    /**
     * @brief getPadding get number of pixels to pad from Layout rect during layout
     * \verbatim
     *  +----------------------+
     *  |       PADDING        |
     *  |   +--------------+   |
     *  | P |   Widget     | P |
     *  | A +--------------+ A |
     *  | D     SPACING      D |
     *  | D +--------------+ D |
     *  | I |   Widget     | I |
     *  | N +--------------+ N |
     *  | G     PADDING      G |
     *  +----------------------+
     * \endverbatim
     * @return padding in pixels
     */
    int getPadding() const;

    /**
     * @brief setPadding set number of pixels to pad from Layout rect during layout
     * \verbatim
     *  +----------------------+
     *  |       PADDING        |
     *  |   +--------------+   |
     *  | P |   Widget     | P |
     *  | A +--------------+ A |
     *  | D     SPACING      D |
     *  | D +--------------+ D |
     *  | I |   Widget     | I |
     *  | N +--------------+ N |
     *  | G     PADDING      G |
     *  +----------------------+
     * \endverbatim
     * @return
     */
    void setPadding(int value);

protected:

    virtual const Rect getBoundaries() const;

    bool getDrawFrame() const;
    void setDrawFrame(bool value);

    LayoutBaseWidget(Layout &layoutVal, const cv::Point& pos);

    virtual bool isDuringUpdate() const;
    virtual void recalc();

    int padding;

private:
    virtual void setDirtyLayout();
};

}
#endif // LAYOUTBASEWIDGET_H
