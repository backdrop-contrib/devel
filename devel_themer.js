if (Drupal.jsEnabled) {
  $(document).ready(function () {
    lastObj = false;
    thmrSpanified = false;
    //blocks = new Array('DIV', 'P', 'ADDRESS', 'BLOCKQUOTE', 'CENTER', 'DIR', 'DL', 'FIELDSET', 'FORM', 'H1', 'H2', 'H3', 'H4', 'H5', 'H6', 'HR', 'ISINDEX', 'MENU', 'NOFRAMES', 'NOSCRIPT', 'OL', 'PRE', 'TABLE', 'UL',  'DD', 'DT', 'FRAMESET', 'LI', 'TBODY', 'TD', 'TFOOT', 'TH', 'THEAD', 'TR');
    $('span.thmr_call')
    .hover(function () {
      if (themerEnabled && this.parentNode.nodeName != 'BODY' && $(this).attr('thmr_curr') != 1) {
        $(this).css('outline', 'red solid 1px');
      }
    },
    function () {
      if (themerEnabled && $(this).attr('thmr_curr') != 1) {
        $(this).css('outline', 'none');
      }
    });

    var themerEnabled = 0;
    var themerToggle = function () {
      themerEnabled = 1 - themerEnabled;
      $('input', this).attr('checked', themerEnabled ? 'checked' : '');
      $('#themer-popup').css('display', themerEnabled ? 'block' : 'none');
      if (themerEnabled) {
        document.onclick = themerEvent;
        if (lastObj != false) {
          $(lastObj).css('outline', '3px solid #999');
        }
        if (!thmrSpanified) {
          $('span.thmr_call')
            .each(function () {
              // make spans around block elements into block elements themselves
              var kids = $(this).children();
              for(i=0;i<kids.length;i++) {
                //console.log(kids[i].style.display);
                if ($(kids[i]).css('display') != 'inline' && $(kids[i]).is('DIV, P, ADDRESS, BLOCKQUOTE, CENTER, DIR, DL, FIELDSET, FORM, H1, H2, H3, H4, H5, H6, HR, ISINDEX, MENU, NOFRAMES, NOSCRIPT, OL, PRE, TABLE, UL,  DD, DT, FRAMESET, LI, TBODY, TD, TFOOT, TH, THEAD, TR')) {
                  $(this).css('display', 'block');
                }
              }
            });
            thmrSpanified = true;
        }
      }
      else {
        document.onclick = null;
        if (lastObj != false) {
          $(lastObj).css('outline', 'none');
        }
      }
    };
    $(Drupal.settings.thmr_popup)
      .appendTo($('body'));

    $('<div id="themer-toggle"><input type="checkbox" />Themer Info</div>')
      .appendTo($('body'))
      .click(themerToggle);

    $('#themer-popup').draggable({
      opacity: .6,
          handle: $('#themer-popup .topper')
    });
    // close box
    $('#themer-popup .topper .close').click(function() {
      themerToggle();
    });
  });
}

function themerHilight(obj) {
  // hilight the current object (and un-highlight the last)
  if (lastObj != false) {
    $(lastObj).css('outline', 'none').attr('thmr_curr', 0);
  }
  $(obj).css('outline', '#999 solid 3px').attr('thmr_curr', 1);
  lastObj = obj;
}

function themerDoIt(obj) {
  //console.log(obj);
  if (thmrInPop(obj)) {
    return true;
  }
  var objs = thmrFindParents(obj);
  if (objs.length) {
    themerHilight(objs[0]);
    thmrRebuildPopup(objs);
  }
  return false;
}

function thmrInPop(obj) {
  //is the element in either the popup box or the toggle div?
  if (obj.id == "themer-popup" || obj.id == "themer-toggle") return true;
  if (obj.parentNode) {
    while (obj = obj.parentNode) {
      if (obj.id=="themer-popup" || obj.id == "themer-toggle") return true;
    }
  }
  return false;
}

function themerEvent(e) {
  if (!e) {
    var e = window.event;
  };
  if (e.target) var tg = e.target;
  else if (e.srcElement) var tg = e.srcElement;
  return themerDoIt(tg);
}

/**
 * Find all parents with class="thmr_call"
 */
function thmrFindParents(obj) {
  var parents = new Array();
  if (obj.className == 'thmr_call') {
    parents[parents.length] = obj;
  }
  if (obj && obj.parentNode) {
    while (obj = obj.parentNode) {
      if (obj.className == 'thmr_call') {
        parents[parents.length] = obj;
      }
    }
  }
  return parents;
}

/**
 * Check to see if object is a block element
 */
function thmrIsBlock(obj) {
  if (obj.style.display == 'block') {
    return true;
  }
  else if (obj.style.display == 'inline' || obj.style.display == 'none') {
    return false;
  }
  if (obj.tagName != undefined) {
    var i = blocks.length;
    if (i > 0) {
      do {
        if (blocks[i] === obj.tagName) {
          return true;
        }
      } while (i--);
    }
  }
  return false;
}

function thmrRefreshCollapse() {
  $('#themer-popup .devel-obj-output dt').each(function() {
      $(this).toggle(function() {
            $(this).parent().children('dd').show();
          }, function() {
            $(this).parent().children('dd').hide();
          });
    });
  $('#themer-popup .devel-obj-output dd').hide();
}

/**
 * Rebuild the popup
 *
 * @param objs
 *   The array of the current object and its parents. Current object is first element of the array
 */
function thmrRebuildPopup(objs) {
  // rebuild the popup box
  var id = objs[0].id;
  // vars is the settings array element for this theme item
  var vars = Drupal.settings[id];
  // strs is the translatable strings
  var strs = Drupal.settings.thmrStrings;
  var type = vars.type;
  var key = vars.name;

  // clear out the initial "click on any element" starter text
  $('#themer-popup div.starter').empty();

  if (type == 'func') {
    // populate the function name
    $('#themer-popup dd.key').empty().prepend('<a href="'+ strs.api_site +'api/search/'+ strs.drupal_version +'/'+ key +'" title="'+ strs.drupal_api_docs +'">'+ key +'()</a>');
    $('#themer-popup dt.key-type').empty().prepend(strs.function_called);
  }
  else {
    // populate the template name
    $('#themer-popup dd.key').empty().prepend(key);
    $('#themer-popup dt.key-type').empty().prepend(strs.template_called);
  }

  // parents
  var parents = '';
  parents = strs.parents +' <span class="parents">';
  for(i=1;i<objs.length;i++) {
    var pvars = Drupal.settings[objs[i].id];
    parents += i!=1 ? '&lt; ' : '';
    // populate the parents
    // each parent is wrapped with a span containing a 'trig' attribute with the id of the element it represents
    parents += '<span class="parent" trig="'+ objs[i].id +'">'+ pvars.name +'</span> ';
  }
  parents += '</span>';
  // stick the parents spans in the #parents div
  $('#themer-popup #parents').empty().prepend(parents);
  $('#themer-popup span.parent').click(function() {
    // make them clickable
    $('#'+ $(this).attr('trig')).each(function() { themerDoIt(this) });
  })
  .hover(function() {
      // make them highlight their element on mouseover
      $('#'+ $(this).attr('trig')).trigger('mouseover');
    },
    function() {
      // and unhilight on mouseout
      $('#'+ $(this).attr('trig')).trigger('mouseout');
    });

  if (vars == undefined) {
    // if there's no item in the settings array for this element
    $('#themer-popup dd.candidates').empty();
    $('#themer-popup div.attributes').empty();
    $('#themer-popup div.used').empty();
    $('#themer-popup div.duration').empty();
  }
  else {
    $('#themer-popup div.duration').empty().prepend('<dt>' + strs.duration + '</dt><dd>' + vars.duration + ' ms</dd>');
    if (type == 'func') {
      if (vars.candidates != undefined && vars.candidates.length != 0) {
        // populate the candidates
        $('#themer-popup dt.candidates-type').empty().prepend(strs.candidate_functions);
        $('#themer-popup dd.candidates').empty().prepend(vars.candidates.join(', '));
      }
      $('#themer-popup div.attributes').empty().prepend('<h4>'+ strs.function_arguments + '</h4>' + vars.args);
      $('#themer-popup div.used').empty();
    }
    else {
      $('#themer-popup dt.candidates-type').empty().prepend(strs.candidate_files);
      $('#themer-popup dd.candidates').empty().prepend(vars.candidates.join(", "));

      $('#themer-popup div.attributes').empty().prepend('<h4>'+ strs.template_variables + '</h4>' + vars.args);
      $('#themer-popup div.used').empty().prepend('<dt>'+ strs.file_used  +'</dt><dd>'+ vars.used +'</dd>');
    }
    thmrRefreshCollapse();
  }
}