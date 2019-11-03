// Load the components you want from here.
// import <CustomComponent> from '@/components/<CustomComponent>';
// The @ sign will resolve to Studio's web source directory.
// Otherwise, use the NPM name for the component and it will get loaded from the library, like:
// import Lightbox from 'lightbox2';
//
// Leaving the extension off of the import will preferentially load the .js file.  If you want it to load the .vue file,
// you will need to specifically add the extension.
//
// While we migrate to the new Vue style loading, we will have to load our own vue components as needed.  To do that,
// create your .vue file, then create a similarly named .js file which will load the .vue file and use the
// VueInitializer to scope load the vue file just in the needed places.  This helps to insert Vue components onto
// a page without having to have the whole page controlled by Vue, which in our case wont work because of so many script
// and style inlined in the page (which Vue does not like since it wants to treat HTML just as templates).
//
// See the @/components/NotificationMessage for an example.
// This is the special GlobalExporter.js file that MUST be included with every entrypoint.  The reasoning for this is
// that by including this file in every entrypoint, it will get sucked out of each entrypoint file and placed into the
// common.bundle.js.  This file is then loaded *before* all other content due to the nature of the legacy application
// code needing some of these libraries during page rendering.  Normally in a SPA, it treats the page as a template and
// renders everything post DOM load, which is why typically all the webpack files are included on the bottom of the
// page.  We aren't quite there yet.  Once we are, it wont matter how we end up bundling this.
import GlobalExporter from 'digitalfusion/common/GlobalExporter';
//GlobalExporter.standardGlobalExports()

import LogUtil from 'digitalfusion/common/util/LogUtil'
const logger = LogUtil({logger: 'entrypoints.studio', caller: 'studio'})
logger.setDefaultLevel(logger.levels.DEBUG); //TODO set based on production/dev

// Sample syntax for adjusting log level (see LogUtil.js)
// Note: For development, do this in the console and the setting will persist in localStorage
//logger.setLevelForModules('common', 'ERROR')

import DomUtil from 'digitalfusion/common/util/DomUtil';
import {Cookie} from 'digitalfusion/common/Cookie';
import {CsrfTokenUtil} from 'digitalfusion/common/CsrfTokenUtil';
import {DatePicker} from 'digitalfusion/common/DatePicker';
import {Keybindings} from 'digitalfusion/common/util/Keybindings';
import {ModalboxUtil} from 'digitalfusion/studio/common/Modal';
import {UICommon} from 'digitalfusion/common/UICommon';
import {Util} from 'digitalfusion/common/UICommon';
import Utils from 'digitalfusion/common/util/Utils';
import ScreenProtection from 'digitalfusion/common/watermark/ScreenProtection';
import WatermarkValidation from 'digitalfusion/common/watermark/WatermarkValidation';
import store from 'store/studio/store';
import {VueInitializer, VueInitializerConfig} from "../../digitalfusion/common/vue/VueInitializer";
import DfConstantsPlugin from 'digitalfusion/common/DfConstantsPlugin';
import TooltipDirective from 'digitalfusion/common/vue/TooltipDirective';
import TruncateDirective from "../../digitalfusion/common/vue/TruncateDirective"
import WebMethodsPlugin from 'digitalfusion/common/webmethods/WebMethodsPlugin';
import {Editor, ProofImage, ProofImageKeys, ProofCompare, ProofCompareKeys, Proofsheet, ProofsheetKeys, setupProofsheetSelectable} from 'digitalfusion/studio/editor/Editor'
import {AccountLogo} from 'digitalfusion/studio/settings/AccountLogo'
import Activity from "../../digitalfusion/studio/activity/Activity";
import Approval from "../../digitalfusion/studio/approval/Approval";
import {AssetSearch} from '../../digitalfusion/studio/search/AssetSearch.js';
import {Assignment} from 'digitalfusion/studio/assignment/Assignment'
import {Bin} from 'digitalfusion/studio/bin/Bin'
import {Collection} from 'digitalfusion/studio/collection/Collection'
import {Contact, ContactList, ContactPoint, ContactSearch, EmailGroup} from 'digitalfusion/studio/contacts/Contacts'
import {SelectSet} from 'digitalfusion/studio/selectset/SelectSet'
import {Library, GroupSelection, Folder, Project, ProjectSettings} from 'digitalfusion/studio/library/Library'
import {Messenger} from 'digitalfusion/studio/messenger/Messenger'
import {Notification, setupInboxSelectable} from "../../digitalfusion/studio/inbox/Inbox";
import {Metadata} from 'digitalfusion/studio/metadata/Metadata'
import SelectLevel from 'digitalfusion/common/selectset/SelectLevel'
import {StudioUiSetupUtil} from 'digitalfusion/studio/setup/SetupUtil'
import {Paging} from 'digitalfusion/studio/paging/Paging'
import {Portfolio} from 'digitalfusion/studio/portfolio/Portfolio'
import {ProjectSearch} from 'digitalfusion/studio/search/ProjectSearch'
import {PermissionsOverlay, newVuePermissionsModel, Grant, Permissions, PermissionSet, PermissionGroups, PermissionModes, Subjects, Rights } from 'digitalfusion/studio/permissions/Permissions'
import {UIStudio, StudioVideo, SelectableUtil, Wizard} from 'digitalfusion/studio/UIStudio'
import {StudioUpload} from 'digitalfusion/studio/upload/StudioUpload'
import {UserSetting} from 'digitalfusion/studio/user/UserSetting'
import Video from "digitalfusion/common/video/Video";
import WatermarkPreview from "digitalfusion/common/watermark/WatermarkPreview";
import {ZoomStateSingleton} from "digitalfusion/common/preview/zoom/ZoomState"

import DfSupportWidget from 'digitalfusion/studio/common/SupportWidget.vue';

import DfTodoApp from 'digitalfusion/studio/todo/TodoApp.vue';

import DfDisclosure from 'digitalfusion/common/disclosure/Disclosure.vue';
import DfDisclosureHead from 'digitalfusion/common/disclosure/DisclosureHead.vue';
import DfDisclosureBody from 'digitalfusion/common/disclosure/DisclosureBody.vue';

import DfAnnotationControls from 'digitalfusion/studio/topics/annotations/AnnotationControls'
import DfLoginAccessHistory from 'digitalfusion/studio/access_history/LoginAccessHistory.vue';
import DfAccessHistoryDetailsConfirmation from 'digitalfusion/studio/access_history/AccessHistoryDetailsConfirmation.vue';
import DfActivityErrors from 'digitalfusion/studio/activity/ActivityErrors.vue';
import DfApprovalImagesReset from 'digitalfusion/studio/approval/ApprovalImagesReset.vue';
import DfAutocompleteStatic from 'digitalfusion/common/AutocompleteStatic.vue';
import DfBadgeLoader from 'digitalfusion/studio/common/BadgeLoader.vue';
import DfBadgeCount from 'digitalfusion/studio/common/BadgeCount';
import DfBookmarkItem from 'digitalfusion/studio/common/Bookmark.vue';
import DfBookmarks from 'digitalfusion/studio/common/Bookmarks.vue';
import DfCopyProjectUploadKey from 'digitalfusion/studio/project/CopyProjectUploadKey'
import DfAddToBinButton from 'digitalfusion/studio/editor/AddToBinButton';
import DfAddToCollectionButton from 'digitalfusion/studio/editor/AddToCollectionButton';
import DfCreatePdfButton from 'digitalfusion/studio/editor/CreatePdfButton';
import DfSendMessengerButton from 'digitalfusion/studio/editor/SendMessengerButton';
import DfPublishButton from 'digitalfusion/studio/editor/PublishButton';
import DfViewFullsizeButton from 'digitalfusion/studio/editor/ViewFullsizeButton';
import DfFindInProject from 'digitalfusion/studio/search/FindInProject.vue';
import DfMetadataCsvImport from 'digitalfusion/studio/metadata/MetadataCsvImport.vue';
import DfMessengerAccessHistory from 'digitalfusion/studio/messenger/MessengerAccessHistory';
import DfProjectActivity from 'digitalfusion/studio/ProjectActivityIndicator.vue';
import DfSharelink from 'digitalfusion/studio/common/Sharelink.vue';
import DfVideoplayer from 'digitalfusion/common/video/Videoplayer.vue';
import DfPreview from 'digitalfusion/studio/common/Preview';
import DfThumbPreview from 'digitalfusion/common/preview/thumb/ThumbPreview'
import DfPreviewCanvas from 'digitalfusion/common/preview/PreviewCanvas'
import Df1UpPreview from 'digitalfusion/common/preview/1UpPreview'
import DfComponentMonitor from 'digitalfusion/common/preview/monitor/ComponentMonitor'
import DfZoomNav from 'digitalfusion/common/preview/zoom/ZoomNav'
import DfProtectedImage from 'digitalfusion/studio/common/ProtectedImage';
import DfWatermarkWidget from 'digitalfusion/studio/watermark/WatermarkWidget.vue';
import DfWatermarkSettings from 'digitalfusion/studio/watermark/WatermarkSettings.vue';
import DfWatermarkModal from 'digitalfusion/studio/watermark/WatermarkModal';
import DfVspConfig from 'digitalfusion/studio/vsp/VspConfig';
import DfVspToggle from 'digitalfusion/studio/vsp/VspToggle';
import DfAssetRenameModal from 'digitalfusion/studio/asset/AssetRenameModal.vue';
import DfEditorAssetSelections from 'digitalfusion/studio/editor/EditorAssetSelections'
import DfProjectUploadModalOld from 'digitalfusion/studio/editor/ProjectUploadModalOld'
import DfProjectImportMessengerModal from 'digitalfusion/studio/editor/ProjectImportMessengerModal'
import DfProjectUploadKeyModal from 'digitalfusion/studio/project/ProjectUploadKeyModal'
import DfProjectUploadModal from 'digitalfusion/studio/upload/ProjectUploadModal'
import DfUserPermissions from 'digitalfusion/studio/permissions/UserPermissions'
import DfApiKeyManager from 'digitalfusion/studio/account/ApiKeyManager'
import DfApplicationPasswordManager from 'digitalfusion/studio/user/ApplicationPasswordManager'
import DfApiKeyModal from 'digitalfusion/studio/account/ApiKeyModal'
import DfMetadataOverlay from 'digitalfusion/studio/metadata/MetadataOverlay.vue';
import DfMetadataEditor from 'digitalfusion/studio/metadata/MetadataEditor.vue';
import DfNodeMetadataLookups from 'digitalfusion/studio/lookup/NodeMetadataLookups.vue';
import DfPublishOptions from 'digitalfusion/studio/publish/options/PublishOptions.vue';
import DfPublishHistory from 'digitalfusion/studio/publish/history/PublishHistory.vue';
import DfPager from 'digitalfusion/common/Pager.vue';
import DfMessage from 'digitalfusion/common/Message.vue';
import DfWorkingSpinner from 'digitalfusion/common/WorkingSpinner.vue';
import DfWorkingBanner from 'digitalfusion/common/WorkingBanner.vue';
import DfSortableSelector from 'digitalfusion/common/SortableSelector.vue';
import DfComponentsDemo from 'digitalfusion/demo/DemoComponents.vue';
import DfLibrary from 'digitalfusion/studio/controller/Library.vue';
import DfEditor from 'digitalfusion/studio/controller/Editor.vue';
import DfInspector from 'digitalfusion/studio/inspector/Inspector.vue';
import DfInspectorTabs from 'digitalfusion/studio/inspector/InspectorTabs.vue';
import DfAssetInspectorInfo from 'digitalfusion/studio/inspector/AssetInspectorInfo.vue';
import DfAssetInspectorHistory from 'digitalfusion/studio/inspector/AssetInspectorHistory.vue';
import DfAssetInspectorMetadataEdit from 'digitalfusion/studio/inspector/AssetInspectorMetadataEdit.vue';
import DfAssetInspectorMetadataFull from 'digitalfusion/studio/inspector/AssetInspectorMetadataFull.vue';
import DfAssetInspectorTopics from 'digitalfusion/studio/inspector/AssetInspectorTopics.vue';
import DfProjectInspectorInfo from 'digitalfusion/studio/inspector/ProjectInspectorInfo.vue';
import DfCollectionInspectorInfo from 'digitalfusion/studio/inspector/CollectionInspectorInfo.vue';
import DfFileInspectorInfo from 'digitalfusion/studio/inspector/FileInspectorInfo.vue';
import DfProofsheetViewOnly from 'digitalfusion/studio/common/ProofsheetViewOnly.vue';
import DfFolderPicker from 'digitalfusion/studio/picker/FolderPicker.vue';
import DfFolderPickerForm from 'digitalfusion/studio/picker/FolderPickerForm.vue';
import DfCollectionPicker from 'digitalfusion/studio/picker/CollectionPicker.vue';
import DfNewProject from 'digitalfusion/studio/create/NewProject.vue';
import DfNewFolderForm from 'digitalfusion/studio/create/NewFolderForm.vue';
import DfNewCollection from 'digitalfusion/studio/create/NewCollection.vue';
import DfAddToCollectionForm from 'digitalfusion/studio/create/AddToCollectionForm.vue';
import DfLibraryMoveForm from 'digitalfusion/studio/LibraryMoveForm.vue';
import DfDateTime from 'digitalfusion/common/vue/DateTime.vue';
import DfMetadataUnsavedChangesConfirmation from 'digitalfusion/studio/metadata/MetadataUnsavedChangesConfirmation'
import DfMetadataSavingNotice from 'digitalfusion/studio/metadata/MetadataSavingNotice'
import DfAccountSwitcher from 'digitalfusion/studio/account/AccountSwitcher'
import DfMessengerTopicSelector from 'digitalfusion/studio/topics/studio/MessengerTopicSelector'
import DfSetupsFilter from 'digitalfusion/studio/setup/SetupsFilter'

import DfSelectedImagesAPI from 'store/modules/common/selectedImagesAPI';
import DfInspectorAPI from 'store/modules/common/inspectorAPI';
import DfConversationsAPI from 'store/modules/common/conversationsAPI';
import DfHistoryAPI from 'store/modules/studio/historyAPI';
import DfTimerAPI from 'store/modules/common/timerAPI';
import DfEditorAPI from 'store/modules/common/editorAPI';

import DfSimpleFormDemo from 'digitalfusion/demo/DemoSimpleForm'

// Look ma, we can import CSS!... wait what?
// Yes, webpack trickery.  This will basically spit out the CSS into an <entrypoint>.bundle.css which should then be
// included on the page.  A separate common.bundle.css will be generated by extracting any commonly included CSS amongst
// all the entrypoint files.
// Note: You should not directly include any CSS files here, instead add them to the corresponding entrypoint bundle.
import 'styles/bundles/common.global.css';
import 'styles/bundles/studio.global.css';

import {initEditorSpec,StudioEditorSpec} from 'store/modules/common/EditorSpec'
import {useStudioMetadataEditorSpec} from 'digitalfusion/studio/metadata/MetadataEditorSpec'

/* Constants */
const moduleName = "studio";
const plugins = [
  DfConstantsPlugin,
  TooltipDirective,
  TruncateDirective,
  WebMethodsPlugin
];
const components = [
  DfAnnotationControls,
  DfSupportWidget,
  DfLoginAccessHistory,
  DfAccessHistoryDetailsConfirmation,
  DfActivityErrors,
  DfApprovalImagesReset,
  DfAutocompleteStatic,
  DfBadgeLoader,
  DfBadgeCount,
  DfBookmarkItem,
  DfBookmarks,
  DfCopyProjectUploadKey,
  DfAddToBinButton,
  DfAddToCollectionButton,
  DfCreatePdfButton,
  DfSendMessengerButton,
  DfPublishButton,
  DfViewFullsizeButton,
  DfDisclosure,
  DfDisclosureHead,
  DfDisclosureBody,
  DfFindInProject,
  DfAssetRenameModal,
  DfEditorAssetSelections,
  DfMessengerAccessHistory,
  DfMetadataOverlay,
  DfMetadataEditor,
  DfNodeMetadataLookups,
  DfMetadataCsvImport,
  DfProjectActivity,
  DfSharelink,
  DfVideoplayer,
  DfPreview,
  DfThumbPreview,
  DfPreviewCanvas,
  Df1UpPreview,
  DfComponentMonitor,
  DfZoomNav,
  DfProtectedImage,
  DfWatermarkWidget,
  DfWatermarkSettings,
  DfWatermarkModal,
  DfVspConfig,
  DfVspToggle,
  DfPublishOptions,
  DfPublishHistory,
  DfPager,
  DfMessage,
  DfWorkingSpinner,
  DfWorkingBanner,
  DfSortableSelector,
  DfComponentsDemo,
  DfLibrary,
  DfEditor,
  DfFolderPicker,
  DfFolderPickerForm,
  DfCollectionPicker,
  DfNewProject,
  DfNewFolderForm,
  DfNewCollection,
  DfAddToCollectionForm,
  DfLibraryMoveForm,
  DfInspector,
  DfInspectorTabs,
  DfAssetInspectorInfo,
  DfAssetInspectorHistory,
  DfAssetInspectorMetadataEdit,
  DfAssetInspectorMetadataFull,
  DfAssetInspectorTopics,
  DfProjectInspectorInfo,
  DfProjectImportMessengerModal,
  DfProjectUploadModalOld,
  DfProjectUploadKeyModal,
  DfApiKeyModal,
  DfCollectionInspectorInfo,
  DfFileInspectorInfo,
  DfProofsheetViewOnly,
  DfDateTime,
  DfMetadataSavingNotice,
  DfMetadataUnsavedChangesConfirmation,
  DfSimpleFormDemo,
  DfProjectUploadModal,
  DfUserPermissions,
  DfAccountSwitcher,
  DfMessengerTopicSelector,
  DfSetupsFilter,
<<<<<<< HEAD
  DfApiKeyManager,
  DfApplicationPasswordManager
=======
  DfTodoApp
>>>>>>> important changes before reset
];
const vueInitializerConfig = new VueInitializerConfig(moduleName, store, plugins, components);
logger.info(`Loading ${moduleName} entrypoint...`);
const vueInitializer = new VueInitializer(vueInitializerConfig)

/* Exports */
export function activateContainer(selector, options) {
  return vueInitializer.activate(selector, options);
}

export function getVueStore(){
  return vueInitializerConfig.getStore();
}

// These need to run once all Webpack code has been loaded and setup.
// The page will not be ready yet, so these should be non visual preparations.
// Visual ones should be places in the addLoadEvent section below.
{
  CsrfTokenUtil.init(
      DF_CONSTANTS.USER_CONSTANTS.User.csrfParamName,
      DF_CONSTANTS.USER_CONSTANTS.User.csrfToken
  );

  GlobalExporter.exportGlobally('Activity', Activity);
  GlobalExporter.exportGlobally('AccountLogo', AccountLogo);
  GlobalExporter.exportGlobally('Approval', Approval);
  GlobalExporter.exportGlobally('AssetSearch', AssetSearch);
  GlobalExporter.exportGlobally('Assignment', Assignment);
  GlobalExporter.exportGlobally('Bin', Bin);
  GlobalExporter.exportGlobally('Cookie', Cookie);
  GlobalExporter.exportGlobally('Collection', Collection);
  GlobalExporter.exportGlobally('Contact', Contact);
  GlobalExporter.exportGlobally('ContactList', ContactList);
  GlobalExporter.exportGlobally('ContactPoint', ContactPoint);
  GlobalExporter.exportGlobally('ContactSearch', ContactSearch);
  GlobalExporter.exportGlobally('EmailGroup', EmailGroup);
  GlobalExporter.exportGlobally('CsrfTokenUtil', CsrfTokenUtil); //used by modalbox
  GlobalExporter.exportGlobally('DatePicker', DatePicker);
  GlobalExporter.exportGlobally('Editor', Editor);
  GlobalExporter.exportGlobally('Notification', Notification);
  GlobalExporter.exportGlobally('setupInboxSelectable', setupInboxSelectable);
  GlobalExporter.exportGlobally('Library', Library);
  GlobalExporter.exportGlobally('GroupSelection', GroupSelection);
  GlobalExporter.exportGlobally('Folder', Folder);
  GlobalExporter.exportGlobally('Messenger', Messenger);
  GlobalExporter.exportGlobally('Metadata', Metadata);
  GlobalExporter.exportGlobally('Portfolio', Portfolio);
  GlobalExporter.exportGlobally('ProofImage', ProofImage);
  GlobalExporter.exportGlobally('ProofImageKeys', ProofImageKeys);
  GlobalExporter.exportGlobally('ProofCompare', ProofCompare);
  GlobalExporter.exportGlobally('ProofCompareKeys', ProofCompareKeys);
  GlobalExporter.exportGlobally('Proofsheet', Proofsheet);
  GlobalExporter.exportGlobally('ProofsheetKeys', ProofsheetKeys);
  GlobalExporter.exportGlobally('setupProofsheetSelectable', setupProofsheetSelectable);
  GlobalExporter.exportGlobally('Keybindings', Keybindings);
  GlobalExporter.exportGlobally('ModalboxUtil', ModalboxUtil);
  GlobalExporter.exportGlobally('PermissionsOverlay', PermissionsOverlay);
  GlobalExporter.exportGlobally('Grant', Grant);
  GlobalExporter.exportGlobally('newVuePermissionsModel', newVuePermissionsModel);
  GlobalExporter.exportGlobally('Permissions', Permissions);
  GlobalExporter.exportGlobally('PermissionSet', PermissionSet);
  GlobalExporter.exportGlobally('Subjects', Subjects);
  GlobalExporter.exportGlobally('Rights', Rights);
  GlobalExporter.exportGlobally('PermissionGroups', PermissionGroups);
  GlobalExporter.exportGlobally('PermissionModes', PermissionModes);
  GlobalExporter.exportGlobally('Paging', Paging);
  GlobalExporter.exportGlobally('ProjectSearch', ProjectSearch);
  GlobalExporter.exportGlobally('Project', Project);
  GlobalExporter.exportGlobally('ProjectSettings', ProjectSettings);
  GlobalExporter.exportGlobally('SelectableUtil', SelectableUtil);
  GlobalExporter.exportGlobally('SelectLevel', SelectLevel);
  GlobalExporter.exportGlobally('SelectSet', SelectSet);
  GlobalExporter.exportGlobally('StudioUiSetupUtil', StudioUiSetupUtil);
  GlobalExporter.exportGlobally('ScreenProtection', ScreenProtection);
  GlobalExporter.exportGlobally('StudioVideo', StudioVideo);
  GlobalExporter.exportGlobally('UI', UIStudio);
  GlobalExporter.exportGlobally('UICommon', UICommon);
  GlobalExporter.exportGlobally('UserSetting', UserSetting);
  GlobalExporter.exportGlobally('StudioUpload', StudioUpload);
  GlobalExporter.exportGlobally('Util', Util);
  GlobalExporter.exportGlobally('Utils', Utils);
  GlobalExporter.exportGlobally('Video', Video);
  GlobalExporter.exportGlobally('WatermarkPreview', WatermarkPreview);
  GlobalExporter.exportGlobally('WatermarkValidation', WatermarkValidation);
  GlobalExporter.exportGlobally('Wizard', Wizard);
  GlobalExporter.exportGlobally('ZoomState', ZoomStateSingleton)

  // Make the store available globally (especially for non webpack processed Javascript)
  GlobalExporter.exportGlobally('VuexDefaultStore', store);

  DfSelectedImagesAPI.store = store;
  GlobalExporter.exportGlobally('SelectedImagesAPI', DfSelectedImagesAPI);

  DfInspectorAPI.store = store;
  GlobalExporter.exportGlobally('InspectorAPI', DfInspectorAPI);

  DfConversationsAPI.store = store;
  DfConversationsAPI.useStudioConversationsSpec();

  DfHistoryAPI.store = store;
  // At the moment, the value is automatically set internally.
  //  DfHistoryAPI.setAuditCategories(DF_CONSTANTS.SHARED_CONSTANTS.Audit.categories);

  DfTimerAPI.store = store;
  GlobalExporter.exportGlobally('TimerAPI', DfTimerAPI);
  DfTimerAPI.start();

  DfEditorAPI.store = store;
  GlobalExporter.exportGlobally('EditorAPI', DfEditorAPI);

  initEditorSpec(new StudioEditorSpec());
  useStudioMetadataEditorSpec();

  // Initialize the bookmarks.
  store.dispatch('bookmarks/load');

  // init vsp module
  store.dispatch('vsp/init');

  // Save the user settings to the store.
  store.commit('studioUser/setBrowserSettings', Object.assign({}, DF_CONSTANTS.USER_CONSTANTS.Settings));
  store.commit('studioUser/setUserData', Object.assign({}, DF_CONSTANTS.USER_CONSTANTS.User));
  store.commit('studioUser/setAccountData', Object.assign({}, DF_CONSTANTS.USER_CONSTANTS.Account));
}

// These need to run after the page has been fully rendered.
// Some of these should probably be returned as promises.
export let autoloadedVueInstance = null;
DomUtil.addLoadEvent(function(event){
  autoloadedVueInstance = vueInitializer.activateAutoLoads();
  CsrfTokenUtil.amendPageForms();
  Cookie.init({name: "DFUI2"});
  UIStudio.navpathTips();
  UIStudio.toggleFullWidthSetup();
  UIStudio.addPopState();
});
Util.addLoadEvent(function(event){
  UIStudio.setupViewportHeight();
  UIStudio.setupViewportWidth();
});
window.name = "dfstudio_main"