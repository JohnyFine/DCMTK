/*
 *
 *  Copyright (C) 2015, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module: dcmsr
 *
 *  Author: Joerg Riesmeier
 *
 *  Purpose:
 *    classes: DSRSubTemplate
 *
 */


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include "dcmtk/dcmsr/dsrstpl.h"


DSRSubTemplate::DSRSubTemplate(const OFString &templateIdentifier,
                               const OFString &mappingResource,
                               const OFString &mappingResourceUID)
  : DSRDocumentSubTree(),
    DSRTemplateCommon(templateIdentifier, mappingResource, mappingResourceUID)
{
}


DSRSubTemplate::DSRSubTemplate(const DSRSubTemplate &subTemplate)
  : DSRDocumentSubTree(subTemplate),
    DSRTemplateCommon(subTemplate)
{
}


DSRSubTemplate::~DSRSubTemplate()
{
}


DSRSubTemplate *DSRSubTemplate::clone() const
{
    return new DSRSubTemplate(*this);
}


DSRDocumentSubTree *DSRSubTemplate::cloneTree() const
{
    DSRDocumentSubTree *tree = DSRDocumentSubTree::clone();
    if (tree != NULL)
    {
        /* mark root CONTAINER with template identification */
        tree->setTemplateIdentification(getTemplateIdentifier(), getMappingResource(), getMappingResourceUID());
    }
    return tree;
}


void DSRSubTemplate::clear()
{
    DSRDocumentSubTree::clear();
}


OFBool DSRSubTemplate::isValid() const
{
    /* check whether both base classes are valid */
    return DSRDocumentSubTree::isValid() && DSRTemplateCommon::isTemplateIdentificationValid();
}


const DSRDocumentSubTree &DSRSubTemplate::getTree()
{
    /* update the document tree for output (if needed) */
    updateTreeForOutput();
    return *this;
}


OFCondition DSRSubTemplate::insertTemplate(const DSRSubTemplate &subTemplate,
                                           const E_AddMode addMode,
                                           const E_RelationshipType defaultRelType)
{
    /* call the functions doing the real work */
    return insertSubTree(subTemplate.cloneTree(), addMode, defaultRelType, OFTrue /*deleteIfFail*/);
}


// protected methods

void DSRSubTemplate::updateTreeForOutput()
{
    /* mark root CONTAINER with template identification (if applicable) */
    OFCondition result = DSRDocumentSubTree::setTemplateIdentification(getTemplateIdentifier(), getMappingResource(), getMappingResourceUID());
    /* in case of error, output some useful debug information */
    if (result.bad())
    {
        /* since this might fail quite often for a non-root template, use "trace" logger */
        DCMSR_TRACE("DSRSubTemplate::updateTreeForOutput() Problem with setting TID "
            << getTemplateIdentifier() << " (" << getMappingResource() << "): " << result.text());
    }
}