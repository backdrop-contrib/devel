// $Id$

/**
  *  @name    jQuery Logging plugin
  *  @author  Dominic Mitchell
  *  @url     http://happygiraffe.net/blog/archives/2007/09/26/jquery-logging
  */
jQuery.fn.log = function (msg) {
    console.log("%s: %o", msg, this);
    return this;
};
