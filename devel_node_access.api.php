<?php
// $Id$

/**
 * @file
 * Hook provided by the Devel Node Access module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Explain your records in the {node_access} table.
 *
 * In order to help developers and administrators understand the forces
 * that control access to any given node, the DNA module provides the
 * Devel Node Access block, which lists all the grant records in the
 * {node_access} table for that node.
 *
 * However, every Node Access module is free in how it defines and uses the
 * 'realm' and 'gid' fields in its records in the {node_access} table, and
 * it's often difficult to interpret them. This hook passes each record
 * that DNA wants to display, and the owning module is expected to return
 * an explanation of that record.
 *
 * The explanation should not be localized (not be passed through t()), so
 * that administrators seeking help can present English explanations.
 *
 * @param $row
 *   The record from the {node_access} table, as object. The member fields are: 
 *   nid, gid, realm, grant_view, grant_update, grant_delete.
 *
 * @return
 *   A (short!) HTML explanation of the given {node_access} row, to be
 *   displayed in DNA's 'Devel Node Access' block.
 *
 * @see devel_node_access_node_access_explain()
 *
 * @ingroup node_access
 */
function hook_node_access_explain($row) {
  if ($row->realm == 'mymodule_myrealm') {
    if ($row->grant_view) {
      return "Role $row->gid may view this node.";
    }
    else {
      return 'No access.';
    }
  }
}

/**
 * @} End of "addtogroup hooks".
 */
